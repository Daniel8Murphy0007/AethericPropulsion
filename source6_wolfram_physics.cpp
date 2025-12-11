// Wolfram-Enhanced Physics Terms from source6.cpp - UQFF PHYSICS
// 4-File Modularization: UQFF Physics Module
// Generated: November 29, 2025
// Total Physics Classes: 15 (7 helpers + 8 core)

#include <cmath>
#include <string>
#include <map>
#include <memory>
#include <vector>
#include <stdexcept>
#include <iostream>

// ============================================================================
// Physics Constants
// ============================================================================
const double PI = 3.141592653589793;
const double G = 6.67430e-11; // m³/(kg·s²)
const double c = 3.0e8;       // m/s

// ============================================================================
// CelestialBody Structure (from source6.cpp)
// ============================================================================
struct CelestialBody
{
    std::string name;
    double Ms;          // Mass (kg)
    double Rs;          // Radius (m)
    double Rb;          // Bubble radius (heliosphere/magnetosphere, m)
    double Ts_surface;  // Surface temperature (K)
    double omega_s;     // Rotation rate (rad/s)
    double Bs_avg;      // Average surface magnetic field (T)
    double SCm_density; // SCm density (kg/m^3)
    double QUA;         // Trapped Universal Aether charge (C)
    double Pcore;       // Planetary core penetration factor
    double PSCm;        // SCm penetration factor
    double omega_c;     // Cycle frequency (rad/s)
};

// ============================================================================
// PhysicsTerm Base Class
// ============================================================================
class PhysicsTerm
{
public:
    virtual ~PhysicsTerm() = default;
    virtual double compute(double t, const std::map<std::string, double> &params) const = 0;
    virtual std::string getName() const = 0;
    virtual std::string getDescription() const = 0;
    virtual bool validate(const std::map<std::string, double> &params) const { return true; }
};

// ============================================================================
// UQFF HELPER TERMS (7 Classes)
// ============================================================================

// Helper Term 1: Step Function (Heaviside)
class StepFunctionSource6Term : public PhysicsTerm
{
public:
    double compute(double t, const std::map<std::string, double> &params) const override
    {
        auto it_r = params.find("r");
        auto it_rb = params.find("Rb");
        double r = (it_r != params.end()) ? it_r->second : 1e13;
        double Rb = (it_rb != params.end()) ? it_rb->second : 1e7;
        return (r > Rb) ? 1.0 : 0.0;
    }

    std::string getName() const override { return "StepFunctionSource6"; }
    std::string getDescription() const override
    {
        return "Heaviside step: S(r,Rb) = 1 if r>Rb else 0";
    }
};

// Helper Term 2: Reactor Energy (E_react from source6.cpp)
class ReactorEnergySource6Term : public PhysicsTerm
{
public:
    double compute(double t, const std::map<std::string, double> &params) const override
    {
        auto it_rho_scm = params.find("SCm_density");
        auto it_v_scm = params.find("v_SCm");
        auto it_rho_a = params.find("rho_A");
        auto it_kappa = params.find("kappa");

        double rho_SCm = (it_rho_scm != params.end()) ? it_rho_scm->second : 1e15;
        double v_SCm = (it_v_scm != params.end()) ? it_v_scm->second : 0.99 * c;
        double rho_A = (it_rho_a != params.end()) ? it_rho_a->second : 1e-23;
        double kappa = (it_kappa != params.end()) ? it_kappa->second : 0.0005;

        if (rho_A <= 0.0)
            return 0.0;
        return (rho_SCm * v_SCm * v_SCm / rho_A) * std::exp(-kappa * t);
    }

    std::string getName() const override { return "ReactorEnergySource6"; }
    std::string getDescription() const override
    {
        return "E_react = (ρ_SCm × v_SCm² / ρ_A) × exp(-κt)";
    }
};

// Helper Term 3: Magnetic Moment Time-Varying
class MagneticMomentTimeSource6Term : public PhysicsTerm
{
public:
    double compute(double t, const std::map<std::string, double> &params) const override
    {
        auto it_bs = params.find("Bs_avg");
        auto it_omega_c = params.find("omega_c");
        auto it_rs = params.find("Rs");

        double Bs = (it_bs != params.end()) ? it_bs->second : 1e-4;
        double omega_c = (it_omega_c != params.end()) ? it_omega_c->second : 2 * PI / (11 * 365.25 * 24 * 3600);
        double Rs = (it_rs != params.end()) ? it_rs->second : 6.96e8;

        double Bs_t = Bs + 0.4 * std::sin(omega_c * t) + 1e3;
        return Bs_t * std::pow(Rs, 3);
    }

    std::string getName() const override { return "MagneticMomentTimeSource6"; }
    std::string getDescription() const override
    {
        return "μ_s(t) = [B_s + 0.4sin(ω_c×t) + 1000] × R_s³";
    }
};

// Helper Term 4: Gradient Mass/Radius
class GradientMassRadiusSource6Term : public PhysicsTerm
{
public:
    double compute(double t, const std::map<std::string, double> &params) const override
    {
        auto it_ms = params.find("Ms");
        auto it_rs = params.find("Rs");

        double Ms = (it_ms != params.end()) ? it_ms->second : 1.989e30;
        double Rs = (it_rs != params.end()) ? it_rs->second : 6.96e8;

        if (Rs <= 0.0)
            return 0.0;
        return G * Ms / (Rs * Rs);
    }

    std::string getName() const override { return "GradientMassRadiusSource6"; }
    std::string getDescription() const override
    {
        return "∇(M_s/r) = G × M_s / R_s²";
    }
};

// Helper Term 5: Magnetic Jet Field
class MagneticJetFieldSource6Term : public PhysicsTerm
{
public:
    double compute(double t, const std::map<std::string, double> &params) const override
    {
        auto it_omega_c = params.find("omega_c");
        double omega_c = (it_omega_c != params.end()) ? it_omega_c->second : 2 * PI / (11 * 365.25 * 24 * 3600);

        return 1e-3 + 0.4 * std::sin(omega_c * t) + 1e3;
    }

    std::string getName() const override { return "MagneticJetFieldSource6"; }
    std::string getDescription() const override
    {
        return "B_j(t) = 10⁻³ + 0.4sin(ω_c×t) + 1000";
    }
};

// Helper Term 6: Omega Spin Modulation
class OmegaSpinModulationSource6Term : public PhysicsTerm
{
public:
    double compute(double t, const std::map<std::string, double> &params) const override
    {
        auto it_omega_s = params.find("omega_s");
        auto it_omega_c = params.find("omega_c");

        double omega_s = (it_omega_s != params.end()) ? it_omega_s->second : 2.5e-6;
        double omega_c = (it_omega_c != params.end()) ? it_omega_c->second : 2 * PI / (11 * 365.25 * 24 * 3600);

        return omega_s - 0.4e-6 * std::sin(omega_c * t);
    }

    std::string getName() const override { return "OmegaSpinModulationSource6"; }
    std::string getDescription() const override
    {
        return "ω_s(t) = ω_s - 0.4×10⁻⁶×sin(ω_c×t)";
    }
};

// Helper Term 7: Magnetic Jet Moment
class MagneticJetMomentSource6Term : public PhysicsTerm
{
public:
    double compute(double t, const std::map<std::string, double> &params) const override
    {
        auto it_omega_c = params.find("omega_c");
        auto it_rs = params.find("Rs");

        double omega_c = (it_omega_c != params.end()) ? it_omega_c->second : 2 * PI / (11 * 365.25 * 24 * 3600);
        double Rs = (it_rs != params.end()) ? it_rs->second : 6.96e8;

        double Bj = 1e-3 + 0.4 * std::sin(omega_c * t) + 1e3;
        return Bj * std::pow(Rs, 3);
    }

    std::string getName() const override { return "MagneticJetMomentSource6"; }
    std::string getDescription() const override
    {
        return "μ_j(t) = B_j(t) × R_s³";
    }
};

// ============================================================================
// UQFF CORE PHYSICS TERMS (8 Classes)
// ============================================================================

// Physics Term 1: Universal Gravity 1 (Magnetic Dipole)
class UniversalGravity1Source6Term : public PhysicsTerm
{
public:
    double compute(double t, const std::map<std::string, double> &params) const override
    {
        auto it_r = params.find("r");
        auto it_tn = params.find("tn");
        auto it_alpha = params.find("alpha");
        auto it_delta_def = params.find("delta_def");
        auto it_k1 = params.find("k1");

        double r = (it_r != params.end()) ? it_r->second : 1e13;
        double tn = (it_tn != params.end()) ? it_tn->second : t;
        double alpha = (it_alpha != params.end()) ? it_alpha->second : 0.001;
        double delta_def = (it_delta_def != params.end()) ? it_delta_def->second : 0.01;
        double k1 = (it_k1 != params.end()) ? it_k1->second : 1.5;

        if (r <= 0.0)
            return 0.0;

        // Calculate μ_s and ∇(M_s/r) using helpers
        MagneticMomentTimeSource6Term mu_s_helper;
        GradientMassRadiusSource6Term grad_helper;

        double mu_s = mu_s_helper.compute(t, params);
        double grad_Ms_r = grad_helper.compute(t, params);
        double defect = 1.0 + delta_def * std::sin(0.001 * t);

        return k1 * mu_s * grad_Ms_r * std::exp(-alpha * t) * std::cos(PI * tn) * defect;
    }

    std::string getName() const override { return "UniversalGravity1Source6"; }
    std::string getDescription() const override
    {
        return "Ug1 = k1 × μ_s(t) × ∇(M_s/r) × exp(-αt) × cos(πt_n) × defect - Magnetic dipole gravity";
    }
};

// Physics Term 2: Universal Gravity 2 (Charge/Superconductor)
class UniversalGravity2Source6Term : public PhysicsTerm
{
public:
    double compute(double t, const std::map<std::string, double> &params) const override
    {
        auto it_r = params.find("r");
        auto it_tn = params.find("tn");
        auto it_k2 = params.find("k2");
        auto it_qa = params.find("QA");
        auto it_delta_sw = params.find("delta_sw");
        auto it_v_sw = params.find("v_sw");
        auto it_hscm = params.find("HSCm");

        double r = (it_r != params.end()) ? it_r->second : 1e13;
        double tn = (it_tn != params.end()) ? it_tn->second : t;
        double k2 = (it_k2 != params.end()) ? it_k2->second : 1.2;
        double QA = (it_qa != params.end()) ? it_qa->second : 1e-10;
        double delta_sw = (it_delta_sw != params.end()) ? it_delta_sw->second : 0.01;
        double v_sw = (it_v_sw != params.end()) ? it_v_sw->second : 5e5;
        double HSCm = (it_hscm != params.end()) ? it_hscm->second : 1.0;

        auto it_ms = params.find("Ms");
        auto it_rb = params.find("Rb");
        auto it_qua = params.find("QUA");

        double Ms = (it_ms != params.end()) ? it_ms->second : 1.989e30;
        double Rb = (it_rb != params.end()) ? it_rb->second : 1.496e13;
        double QUA = (it_qua != params.end()) ? it_qua->second : 1e-11;

        if (r <= 0.0)
            return 0.0;

        ReactorEnergySource6Term ereact_helper;
        double Ereact = ereact_helper.compute(t, params);

        StepFunctionSource6Term step_helper;
        double S = step_helper.compute(t, params);

        double wind_mod = 1.0 + delta_sw * v_sw;

        return k2 * (QA + QUA) * Ms / (r * r) * S * wind_mod * HSCm * Ereact;
    }

    std::string getName() const override { return "UniversalGravity2Source6"; }
    std::string getDescription() const override
    {
        return "Ug2 = k2 × (Q_A+Q_UA) × M_s/r² × S(r,R_b) × wind × H_SCm × E_react - Charge gravity";
    }
};

// Physics Term 3: Universal Gravity 3 (Magnetic Strings)
class UniversalGravity3Source6Term : public PhysicsTerm
{
public:
    double compute(double t, const std::map<std::string, double> &params) const override
    {
        auto it_r = params.find("r");
        auto it_tn = params.find("tn");
        auto it_theta = params.find("theta");
        auto it_k3 = params.find("k3");
        auto it_pcore = params.find("Pcore");

        double r = (it_r != params.end()) ? it_r->second : 1e13;
        double tn = (it_tn != params.end()) ? it_tn->second : t;
        double theta = (it_theta != params.end()) ? it_theta->second : 0.0;
        double k3 = (it_k3 != params.end()) ? it_k3->second : 1.8;
        double Pcore = (it_pcore != params.end()) ? it_pcore->second : 1.0;

        ReactorEnergySource6Term ereact_helper;
        OmegaSpinModulationSource6Term omega_helper;
        MagneticJetFieldSource6Term bj_helper;

        double Ereact = ereact_helper.compute(t, params);
        double omega_s_t = omega_helper.compute(t, params);
        double Bj = bj_helper.compute(t, params);

        return k3 * Bj * std::cos(omega_s_t * t * PI) * Pcore * Ereact;
    }

    std::string getName() const override { return "UniversalGravity3Source6"; }
    std::string getDescription() const override
    {
        return "Ug3 = k3 × B_j × cos(ω_s(t)×t×π) × P_core × E_react - Magnetic strings gravity";
    }
};

// Physics Term 4: Universal Gravity 4 (Reactor/Black Hole)
class UniversalGravity4Source6Term : public PhysicsTerm
{
public:
    double compute(double t, const std::map<std::string, double> &params) const override
    {
        auto it_tn = params.find("tn");
        auto it_rho_v = params.find("rho_v");
        auto it_c_conc = params.find("C_concentration");
        auto it_mbh = params.find("Mbh");
        auto it_dg = params.find("dg");
        auto it_alpha = params.find("alpha");
        auto it_f_fb = params.find("f_feedback");
        auto it_k4 = params.find("k4");

        double tn = (it_tn != params.end()) ? it_tn->second : t;
        double rho_v = (it_rho_v != params.end()) ? it_rho_v->second : 6e-27;
        double C_concentration = (it_c_conc != params.end()) ? it_c_conc->second : 1.0;
        double Mbh = (it_mbh != params.end()) ? it_mbh->second : 8.15e36;
        double dg = (it_dg != params.end()) ? it_dg->second : 2.55e20;
        double alpha = (it_alpha != params.end()) ? it_alpha->second : 0.001;
        double f_feedback = (it_f_fb != params.end()) ? it_f_fb->second : 0.1;
        double k4 = (it_k4 != params.end()) ? it_k4->second : 2.0;

        if (dg <= 0.0)
            return 0.0;

        double decay = std::exp(-alpha * t);
        double cycle = std::cos(PI * tn);

        return k4 * rho_v * C_concentration * Mbh / dg * decay * cycle * (1 + f_feedback);
    }

    std::string getName() const override { return "UniversalGravity4Source6"; }
    std::string getDescription() const override
    {
        return "Ug4 = k4 × ρ_v × C × M_bh/d_g × exp(-αt) × cos(πt_n) × (1+f_fb) - Reactor gravity";
    }
};

// Physics Term 5: Universal Buoyancy (Ubi)
class UniversalBuoyancySource6Term : public PhysicsTerm
{
public:
    double compute(double t, const std::map<std::string, double> &params) const override
    {
        auto it_ugi = params.find("Ugi");
        auto it_beta_i = params.find("beta_i");
        auto it_omega_g = params.find("Omega_g");
        auto it_mbh = params.find("Mbh");
        auto it_dg = params.find("dg");
        auto it_epsilon_sw = params.find("epsilon_sw");
        auto it_rho_sw = params.find("rho_sw");
        auto it_uua = params.find("UUA");
        auto it_tn = params.find("tn");

        double Ugi = (it_ugi != params.end()) ? it_ugi->second : 1e10;
        double beta_i = (it_beta_i != params.end()) ? it_beta_i->second : 0.6;
        double Omega_g = (it_omega_g != params.end()) ? it_omega_g->second : 7.3e-16;
        double Mbh = (it_mbh != params.end()) ? it_mbh->second : 8.15e36;
        double dg = (it_dg != params.end()) ? it_dg->second : 2.55e20;
        double epsilon_sw = (it_epsilon_sw != params.end()) ? it_epsilon_sw->second : 0.001;
        double rho_sw = (it_rho_sw != params.end()) ? it_rho_sw->second : 8e-21;
        double UUA = (it_uua != params.end()) ? it_uua->second : 1.0;
        double tn = (it_tn != params.end()) ? it_tn->second : t;

        if (dg <= 0.0)
            return 0.0;

        double wind_mod = 1.0 + epsilon_sw * rho_sw;
        return -beta_i * Ugi * Omega_g * Mbh / dg * wind_mod * UUA * std::cos(PI * tn);
    }

    std::string getName() const override { return "UniversalBuoyancySource6"; }
    std::string getDescription() const override
    {
        return "Ubi = -β_i × Ug_i × Ω_g × M_bh/d_g × (1+ε_sw×ρ_sw) × UUA × cos(πt_n)";
    }
};

// Physics Term 6: Universal Magnetism (Um - Cosmic Strings)
class UniversalMagnetismSource6Term : public PhysicsTerm
{
public:
    double compute(double t, const std::map<std::string, double> &params) const override
    {
        auto it_tn = params.find("tn");
        auto it_rj = params.find("rj");
        auto it_gamma = params.find("gamma");
        auto it_num_strings = params.find("num_strings");
        auto it_phi_hat = params.find("phi_hat");
        auto it_pscm = params.find("PSCm");

        double tn = (it_tn != params.end()) ? it_tn->second : t;
        double rj = (it_rj != params.end()) ? it_rj->second : params.at("Rb");
        double gamma = (it_gamma != params.end()) ? it_gamma->second : 0.00005;
        double num_strings = (it_num_strings != params.end()) ? it_num_strings->second : 1e9;
        double phi_hat = (it_phi_hat != params.end()) ? it_phi_hat->second : 1.0;
        double PSCm = (it_pscm != params.end()) ? it_pscm->second : 1.0;

        if (rj <= 0.0)
            return 0.0;

        ReactorEnergySource6Term ereact_helper;
        MagneticJetMomentSource6Term mu_j_helper;

        double Ereact = ereact_helper.compute(t, params);
        double mu_j = mu_j_helper.compute(t, params);

        double decay = 1.0 - std::exp(-gamma * t * std::cos(PI * tn));
        double single = mu_j / rj * decay * phi_hat;

        return single * num_strings * PSCm * Ereact;
    }

    std::string getName() const override { return "UniversalMagnetismSource6"; }
    std::string getDescription() const override
    {
        return "Um = μ_j/r_j × [1-exp(-γt×cos(πt_n))] × φ_hat × N_strings × P_SCm × E_react";
    }
};

// Physics Term 7: Spacetime Metric Modulation (A_mu_nu)
class SpacetimeMetricSource6Term : public PhysicsTerm
{
private:
    std::vector<std::vector<double>> g_mu_nu = {
        {1.0, 0.0, 0.0, 0.0},
        {0.0, -1.0, 0.0, 0.0},
        {0.0, 0.0, -1.0, 0.0},
        {0.0, 0.0, 0.0, -1.0}};

public:
    double compute(double t, const std::map<std::string, double> &params) const override
    {
        auto it_tn = params.find("tn");
        auto it_eta = params.find("eta");
        auto it_ts00 = params.find("Ts00");

        double tn = (it_tn != params.end()) ? it_tn->second : t;
        double eta = (it_eta != params.end()) ? it_eta->second : 1e-22;
        double Ts00 = (it_ts00 != params.end()) ? it_ts00->second : 1.27e3 + 1.11e7;

        double mod = eta * Ts00 * std::cos(PI * tn);
        double trace = 0.0;
        for (int i = 0; i < 4; ++i)
        {
            trace += g_mu_nu[i][i] + mod;
        }
        return trace;
    }

    std::string getName() const override { return "SpacetimeMetricSource6"; }
    std::string getDescription() const override
    {
        return "A_μν = g_μν + η×T_s00×cos(πt_n) - Metric tensor modulation trace";
    }
};

// Physics Term 8: Full Unified Field (FU - combines all components)
class FullUnifiedFieldSource6Term : public PhysicsTerm
{
public:
    double compute(double t, const std::map<std::string, double> &params) const override
    {
        // Calculate all Ug terms
        UniversalGravity1Source6Term ug1;
        UniversalGravity2Source6Term ug2;
        UniversalGravity3Source6Term ug3;
        UniversalGravity4Source6Term ug4;

        double Ug1_val = ug1.compute(t, params);
        double Ug2_val = ug2.compute(t, params);
        double Ug3_val = ug3.compute(t, params);
        double Ug4_val = ug4.compute(t, params);
        double sum_Ugi = Ug1_val + Ug2_val + Ug3_val + Ug4_val;

        // Calculate all Ubi terms
        UniversalBuoyancySource6Term ubi;
        std::map<std::string, double> ubi_params = params;

        ubi_params["Ugi"] = Ug1_val;
        double Ubi1 = ubi.compute(t, ubi_params);

        ubi_params["Ugi"] = Ug2_val;
        double Ubi2 = ubi.compute(t, ubi_params);

        ubi_params["Ugi"] = Ug3_val;
        double Ubi3 = ubi.compute(t, ubi_params);

        ubi_params["Ugi"] = Ug4_val;
        double Ubi4 = ubi.compute(t, ubi_params);

        double sum_Ubi = Ubi1 + Ubi2 + Ubi3 + Ubi4;

        // Calculate Um
        UniversalMagnetismSource6Term um;
        double Um_val = um.compute(t, params);

        // Calculate A_mu_nu trace
        SpacetimeMetricSource6Term metric;
        double A_scalar = metric.compute(t, params);

        return sum_Ugi + sum_Ubi + Um_val + A_scalar;
    }

    std::string getName() const override { return "FullUnifiedFieldSource6"; }
    std::string getDescription() const override
    {
        return "FU = Σ(Ug_i) + Σ(Ubi_i) + Um + trace(A_μν) - Complete unified field strength";
    }
};

// ============================================================================
// 15 UQFF PHYSICS CLASSES
// Helpers (7): StepFunction, ReactorEnergy, MagneticMomentTime, GradientMassRadius,
//              MagneticJetField, OmegaSpinModulation, MagneticJetMoment
// Core (8): UniversalGravity1-4, UniversalBuoyancy, UniversalMagnetism,
//           SpacetimeMetric, FullUnifiedField
// ============================================================================
