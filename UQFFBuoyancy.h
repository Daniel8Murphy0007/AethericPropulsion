#ifndef UQFF_BUOYANCY_H
#define UQFF_BUOYANCY_H

#include <vector>
#include <complex>
#include <cmath>
#include <string>
#include <map>

// Constants for UQFF Buoyancy calculations
namespace UQFFConstants
{
    const double PI = 3.141592653589793;
    const double K_R = 1.0;                      // Electrostatic barrier constant
    const double Z_MAX = 1000.0;                 // Max Z for frequency arrays
    const double RHO_VAC_UA = 7.09e-36;          // Vacuum energy density [UA] (J/m³)
    const double RHO_VAC_SCM = 2.27e-35;         // Vacuum energy density [SCM] (J/m³)
    const double NU_THz = 1e12;                  // THz frequency (Hz)
    const double K_Q = 8.99e9;                   // Coulomb constant (N·m²/C²)
    const double H_Z_BASE = 2.268e-18;           // Hubble constant base (s⁻¹)
    const double E_RAD = 0.1554;                 // Radiation energy fraction
    const double T_SF = 3.156e13;                // Star formation timescale (s)
    const double M_SF = 1.5;                     // SFR adjustment factor
    const std::complex<double> I_UNIT(0.0, 1.0); // Imaginary unit
}

// UQFF Buoyancy System Type Enumeration
enum class UQFFBuoyancyType
{
    UNIVERSAL_BUOYANCY,  // U_Bi - Standard universal buoyancy
    UNIVERSAL_INERTIA,   // U_Ii - Universal inertia field
    UNIVERSAL_MAGNETISM, // U_Mi - Universal magnetic field
    COMPRESSED_GRAVITY,  // Compressed UQFF (gravity-like)
    RESONANCE_BUOYANCY,  // Resonance UQFF
    LENR_ENHANCED        // Low Energy Nuclear Reaction enhanced
};

// DPM (Di-Pseudo-Monopole) Variables Structure
struct DPMVars
{
    std::complex<double> rho_vac_ua;  // Vacuum density UA (complex for quantum states)
    std::complex<double> rho_vac_scm; // Vacuum density SCM
    std::complex<double> nu_thz;      // THz frequency
    std::complex<double> k_q;         // Quantum coupling constant
    double delta_r;                   // Radial differential
    double delta_theta;               // Angular differential
    double time;                      // Time parameter

    DPMVars()
        : rho_vac_ua(UQFFConstants::RHO_VAC_UA, 0.0),
          rho_vac_scm(UQFFConstants::RHO_VAC_SCM, 0.0),
          nu_thz(UQFFConstants::NU_THz, 0.0),
          k_q(UQFFConstants::K_Q, 0.0),
          delta_r(1e-10), delta_theta(1e-10), time(0.0) {}
};

// UQFF Buoyancy Core Class
class UQFFBuoyancyCore
{
private:
    DPMVars dpm;
    std::map<std::string, double> scaling_factors;

    // Frequency arrays for 26-dimensional quantum state structure
    std::vector<double> f_UA_prime; // f'_UA[i] for i=1..26
    std::vector<double> f_SCm;      // f_SCm[i] for i=1..26

public:
    UQFFBuoyancyCore()
    {
        initializeFrequencyArrays();
        scaling_factors["default"] = 1.0;
        scaling_factors["LENR"] = 1.2; // Enhanced for LENR scenarios
    }

    // Initialize 26D frequency structure (quantum alphabet)
    void initializeFrequencyArrays()
    {
        f_UA_prime.resize(26);
        f_SCm.resize(26);

        // Base frequencies derived from UQFF framework
        for (int i = 0; i < 26; ++i)
        {
            double z = i + 1.0;
            f_UA_prime[i] = UQFFConstants::H_Z_BASE * z * (1.0 + UQFFConstants::E_RAD);
            f_SCm[i] = f_UA_prime[i] * (1.0 + 1.0 / (UQFFConstants::T_SF * z));
        }
    }

    // Calculate Universal Buoyancy U_Bi
    std::complex<double> calculate_U_Bi(double mass, double radius, double time_sec)
    {
        using namespace UQFFConstants;

        // DPM scaling factor
        double dpm_scale = std::exp(-time_sec / T_SF);

        // Vacuum energy differential
        std::complex<double> delta_vac = dpm.rho_vac_scm - dpm.rho_vac_ua;

        // Buoyancy force calculation
        double volume = (4.0 / 3.0) * PI * std::pow(radius, 3);
        std::complex<double> F_buoyancy = delta_vac * volume * dpm_scale;

        // Convert to buoyancy field strength
        std::complex<double> U_Bi = F_buoyancy / mass;

        return U_Bi;
    }

    // Calculate Universal Inertia U_Ii (resistance to acceleration)
    std::complex<double> calculate_U_Ii(double mass, double acceleration)
    {
        using namespace UQFFConstants;

        // Inertial coupling through vacuum
        std::complex<double> inertial_coupling = I_UNIT * dpm.nu_thz * dpm.rho_vac_ua;

        // Inertia field
        std::complex<double> U_Ii = inertial_coupling * mass * acceleration;

        return U_Ii;
    }

    // Calculate Universal Magnetism U_Mi
    std::complex<double> calculate_U_Mi(double B_field, double volume, int quantum_state = 13)
    {
        if (quantum_state < 1 || quantum_state > 26)
            quantum_state = 13; // Default to middle state

        // Magnetic coupling through quantum state
        double f_effective = (f_UA_prime[quantum_state - 1] + f_SCm[quantum_state - 1]) / 2.0;

        std::complex<double> U_Mi = UQFFConstants::I_UNIT * B_field * volume * f_effective;

        return U_Mi;
    }

    // Master F_U_Bi_i calculation (combines all three)
    std::complex<double> calculate_F_U_Bi_i(double mass, double radius, double B_field,
                                            double time_sec, double acceleration = 0.0)
    {
        std::complex<double> U_Bi = calculate_U_Bi(mass, radius, time_sec);
        std::complex<double> U_Ii = calculate_U_Ii(mass, acceleration);

        double volume = (4.0 / 3.0) * UQFFConstants::PI * std::pow(radius, 3);
        std::complex<double> U_Mi = calculate_U_Mi(B_field, volume);

        // Combined master force
        std::complex<double> F_total = U_Bi + U_Ii + U_Mi;

        return F_total;
    }

    // LENR-enhanced buoyancy (with neutron drop and relativistic coherence)
    std::complex<double> calculate_LENR_buoyancy(double mass, double radius,
                                                 double time_sec, double coherence_factor = 1.0)
    {
        std::complex<double> base_buoyancy = calculate_U_Bi(mass, radius, time_sec);

        // LENR enhancement through coherent quantum states
        double lenr_scaling = scaling_factors["LENR"] * coherence_factor;

        // Neutron drop resonance (simplified model)
        double neutron_resonance = std::cos(UQFFConstants::NU_THz * time_sec) * 0.1;

        std::complex<double> lenr_enhanced = base_buoyancy * lenr_scaling *
                                             (1.0 + neutron_resonance);

        return lenr_enhanced;
    }

    // Get DPM variables (for external modification)
    DPMVars &getDPMVars() { return dpm; }
    const DPMVars &getDPMVars() const { return dpm; }

    // Set scaling factor
    void setScalingFactor(const std::string &key, double value)
    {
        scaling_factors[key] = value;
    }

    double getScalingFactor(const std::string &key) const
    {
        auto it = scaling_factors.find(key);
        return (it != scaling_factors.end()) ? it->second : 1.0;
    }
};

// UQFF Buoyancy System (for specific astrophysical objects)
class UQFFBuoyancySystem
{
private:
    std::string name;
    UQFFBuoyancyType type;
    UQFFBuoyancyCore core;

    // System properties
    double mass;     // kg
    double radius;   // m
    double B_field;  // T (Tesla)
    double distance; // m (from observer)
    double age;      // seconds

public:
    UQFFBuoyancySystem(const std::string &sys_name, UQFFBuoyancyType sys_type,
                       double m, double r, double B = 1e-10, double d = 1e20, double t = 0.0)
        : name(sys_name), type(sys_type), mass(m), radius(r), B_field(B),
          distance(d), age(t) {}

    // Compute all UQFF buoyancy components for this system
    std::map<std::string, std::complex<double>> computeAll()
    {
        std::map<std::string, std::complex<double>> results;

        results["U_Bi"] = core.calculate_U_Bi(mass, radius, age);
        results["U_Ii"] = core.calculate_U_Ii(mass, 0.0); // Zero acceleration at rest
        results["U_Mi"] = core.calculate_U_Mi(B_field,
                                              (4.0 / 3.0) * UQFFConstants::PI * std::pow(radius, 3));
        results["F_U_Bi_i"] = core.calculate_F_U_Bi_i(mass, radius, B_field, age);

        if (type == UQFFBuoyancyType::LENR_ENHANCED)
        {
            results["LENR"] = core.calculate_LENR_buoyancy(mass, radius, age);
        }

        return results;
    }

    // Getters
    std::string getName() const { return name; }
    UQFFBuoyancyType getType() const { return type; }
    UQFFBuoyancyCore &getCore() { return core; }
};

#endif // UQFF_BUOYANCY_H
