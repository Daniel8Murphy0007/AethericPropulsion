// Wolfram-Enhanced Physics Terms from source4.cpp
// Generated: November 29, 2025 (Phase 1 Complete)
// Source: Unified Field Theory Implementation (UQFF + MUGE + Navier-Stokes)
// Total Classes: 24 (19 original + 5 new helper classes)
// Status: PHASE 1 COMPLETE - Helper functions added

#include <cmath>
#include <string>
#include <map>
#include <memory>

// ============================================================================
// UNIVERSAL GRAVITY COMPONENTS (Ug1-Ug4)
// ============================================================================

class UniversalGravity1Term : public PhysicsTerm
{
private:
    double k1 = 1.5;
    double alpha = 0.001;
    double delta_def = 0.01;

public:
    double compute(double t, const std::map<std::string, double> &params) const override
    {
        auto it_mu_s = params.find("mu_s");
        auto it_grad = params.find("grad_Ms_r");
        auto it_tn = params.find("tn");

        double mu_s = (it_mu_s != params.end()) ? it_mu_s->second : 1e20;
        double grad_Ms_r = (it_grad != params.end()) ? it_grad->second : 1e-5;
        double tn = (it_tn != params.end()) ? it_tn->second : t;

        double defect = 1.0 + delta_def * std::sin(0.001 * t);
        return k1 * mu_s * grad_Ms_r * std::exp(-alpha * t) * std::cos(M_PI * tn) * defect;
    }

    std::string getName() const override { return "UniversalGravity1"; }

    std::string getDescription() const override
    {
        return "Ug1: Magnetic dipole-gradient gravity with defect modulation (k1*mu_s*grad(M/r)*exp(-alpha*t)*cos(PI*tn)*defect)";
    }

    bool validate(const std::map<std::string, double> &) const override { return true; }
};

class UniversalGravity2Term : public PhysicsTerm
{
private:
    double k2 = 1.2;
    double QA = 1e-10;
    double delta_sw = 0.01;
    double v_sw = 5e5;
    double HSCm = 1.0;

public:
    double compute(double t, const std::map<std::string, double> &params) const override
    {
        auto it_QUA = params.find("QUA");
        auto it_M = params.find("mass");
        auto it_r = params.find("radius");
        auto it_Ereact = params.find("Ereact");
        auto it_S = params.find("step_function");

        double QUA = (it_QUA != params.end()) ? it_QUA->second : 1e-11;
        double M = (it_M != params.end()) ? it_M->second : 1e30;
        double r = (it_r != params.end()) ? it_r->second : 1e13;
        double Ereact = (it_Ereact != params.end()) ? it_Ereact->second : 1.0;
        double S = (it_S != params.end()) ? it_S->second : 1.0;

        double wind_mod = 1.0 + delta_sw * v_sw;
        return k2 * (QA + QUA) * M / (r * r) * S * wind_mod * HSCm * Ereact;
    }

    std::string getName() const override { return "UniversalGravity2"; }

    std::string getDescription() const override
    {
        return "Ug2: Charge-reactivity gravity with solar wind modulation (k2*(QA+QUA)*M/r^2*S*wind_mod*HSCm*Ereact)";
    }

    bool validate(const std::map<std::string, double> &) const override { return true; }
};

class UniversalGravity3Term : public PhysicsTerm
{
private:
    double k3 = 1.8;

public:
    double compute(double t, const std::map<std::string, double> &params) const override
    {
        auto it_Bj = params.find("Bj");
        auto it_omega_s_t = params.find("omega_s_t");
        auto it_Pcore = params.find("Pcore");
        auto it_Ereact = params.find("Ereact");

        double Bj = (it_Bj != params.end()) ? it_Bj->second : 1e-3;
        double omega_s_t = (it_omega_s_t != params.end()) ? it_omega_s_t->second : 1e-6;
        double Pcore = (it_Pcore != params.end()) ? it_Pcore->second : 1e-3;
        double Ereact = (it_Ereact != params.end()) ? it_Ereact->second : 1.0;

        return k3 * Bj * std::cos(omega_s_t * t * M_PI) * Pcore * Ereact;
    }

    std::string getName() const override { return "UniversalGravity3"; }

    std::string getDescription() const override
    {
        return "Ug3: Magnetic string rotation gravity (k3*Bj*cos(omega_s_t*t*PI)*Pcore*Ereact)";
    }

    bool validate(const std::map<std::string, double> &) const override { return true; }
};

class UniversalGravity4Term : public PhysicsTerm
{
private:
    double k4 = 2.0;
    double rho_v = 6e-27;
    double C_concentration = 1.0;
    double alpha = 0.001;
    double f_feedback = 0.1;

public:
    double compute(double t, const std::map<std::string, double> &params) const override
    {
        auto it_Mbh = params.find("Mbh");
        auto it_dg = params.find("dg");
        auto it_tn = params.find("tn");

        double Mbh = (it_Mbh != params.end()) ? it_Mbh->second : 8.15e36;
        double dg = (it_dg != params.end()) ? it_dg->second : 2.55e20;
        double tn = (it_tn != params.end()) ? it_tn->second : t;

        double decay = std::exp(-alpha * t);
        double cycle = std::cos(M_PI * tn);
        return k4 * rho_v * C_concentration * Mbh / dg * decay * cycle * (1 + f_feedback);
    }

    std::string getName() const override { return "UniversalGravity4"; }

    std::string getDescription() const override
    {
        return "Ug4: Vacuum energy concentration gravity (k4*rho_v*C*Mbh/dg*exp(-alpha*t)*cos(PI*tn)*(1+f_feedback))";
    }

    bool validate(const std::map<std::string, double> &) const override { return true; }
};

// ============================================================================
// UNIVERSAL BUOYANCY, MAGNETISM, AETHER
// ============================================================================

class UniversalBuoyancyTerm : public PhysicsTerm
{
private:
    double beta_i = 0.6;
    double Omega_g = 7.3e-16;
    double epsilon_sw = 0.001;
    double UUA = 1.0;

public:
    double compute(double t, const std::map<std::string, double> &params) const override
    {
        auto it_Ugi = params.find("Ugi");
        auto it_Mbh = params.find("Mbh");
        auto it_dg = params.find("dg");
        auto it_rho_sw = params.find("rho_sw");
        auto it_tn = params.find("tn");

        double Ugi = (it_Ugi != params.end()) ? it_Ugi->second : 1.0;
        double Mbh = (it_Mbh != params.end()) ? it_Mbh->second : 8.15e36;
        double dg = (it_dg != params.end()) ? it_dg->second : 2.55e20;
        double rho_sw = (it_rho_sw != params.end()) ? it_rho_sw->second : 8e-21;
        double tn = (it_tn != params.end()) ? it_tn->second : t;

        double wind_mod = 1.0 + epsilon_sw * rho_sw;
        return -beta_i * Ugi * Omega_g * Mbh / dg * wind_mod * UUA * std::cos(M_PI * tn);
    }

    std::string getName() const override { return "UniversalBuoyancy"; }

    std::string getDescription() const override
    {
        return "Ubi: Universal buoyancy from galactic rotation (-beta_i*Ugi*Omega_g*Mbh/dg*wind_mod*UUA*cos(PI*tn))";
    }

    bool validate(const std::map<std::string, double> &) const override { return true; }
};

class UniversalMagnetismTerm : public PhysicsTerm
{
private:
    double gamma = 0.00005;
    double num_strings = 1e9;

public:
    double compute(double t, const std::map<std::string, double> &params) const override
    {
        auto it_mu_j = params.find("mu_j");
        auto it_rj = params.find("rj");
        auto it_PSCm = params.find("PSCm");
        auto it_Ereact = params.find("Ereact");
        auto it_tn = params.find("tn");

        double mu_j = (it_mu_j != params.end()) ? it_mu_j->second : 1e20;
        double rj = (it_rj != params.end()) ? it_rj->second : 1e13;
        double PSCm = (it_PSCm != params.end()) ? it_PSCm->second : 1e-3;
        double Ereact = (it_Ereact != params.end()) ? it_Ereact->second : 1.0;
        double tn = (it_tn != params.end()) ? it_tn->second : t;

        double decay = 1.0 - std::exp(-gamma * t * std::cos(M_PI * tn));
        double single = mu_j / rj * decay;
        return single * num_strings * PSCm * Ereact;
    }

    std::string getName() const override { return "UniversalMagnetism"; }

    std::string getDescription() const override
    {
        return "Um: Billion magnetic strings (num_strings*mu_j/rj*(1-exp(-gamma*t*cos(PI*tn)))*PSCm*Ereact)";
    }

    bool validate(const std::map<std::string, double> &) const override { return true; }
};

class UniversalAetherTerm : public PhysicsTerm
{
private:
    double eta = 1e-22;
    double Ts00 = 1.27e3 + 1.11e7;

public:
    double compute(double t, const std::map<std::string, double> &params) const override
    {
        auto it_tn = params.find("tn");
        double tn = (it_tn != params.end()) ? it_tn->second : t;

        double mod = eta * Ts00 * std::cos(M_PI * tn);
        // Return trace of perturbed metric: (1+mod) + (-1+mod) + (-1+mod) + (-1+mod) = -2 + 4*mod
        return -2.0 + 4.0 * mod;
    }

    std::string getName() const override { return "UniversalAether"; }

    std::string getDescription() const override
    {
        return "A_mu_nu: Cosmic aether metric tensor trace (Minkowski + eta*Ts00*cos(PI*tn))";
    }

    bool validate(const std::map<std::string, double> &) const override { return true; }
};

class UnifiedFieldTerm : public PhysicsTerm
{
public:
    double compute(double t, const std::map<std::string, double> &params) const override
    {
        // This would combine Ug1-4, Ubi1-4, Um, and A_mu_nu
        // For simplicity, return sum of parameter "sum_Ugi", "sum_Ubi", "Um", "A_scalar"
        auto it_Ugi = params.find("sum_Ugi");
        auto it_Ubi = params.find("sum_Ubi");
        auto it_Um = params.find("Um");
        auto it_A = params.find("A_scalar");

        double sum_Ugi = (it_Ugi != params.end()) ? it_Ugi->second : 0.0;
        double sum_Ubi = (it_Ubi != params.end()) ? it_Ubi->second : 0.0;
        double Um = (it_Um != params.end()) ? it_Um->second : 0.0;
        double A_scalar = (it_A != params.end()) ? it_A->second : 0.0;

        return sum_Ugi + sum_Ubi + Um + A_scalar;
    }

    std::string getName() const override { return "UnifiedField"; }

    std::string getDescription() const override
    {
        return "FU: Complete unified field (sum_Ugi + sum_Ubi + Um + A_mu_nu_trace)";
    }

    bool validate(const std::map<std::string, double> &) const override { return true; }
};

// ============================================================================
// COMPRESSED MUGE EQUATION (9 Terms)
// ============================================================================

class CompressedMUGETerm : public PhysicsTerm
{
private:
    double G = 6.67430e-11;
    double c = 3.0e8;
    double H0 = 2.269e-18;
    double Lambda = 1.1e-52;
    double hbar = 1.0546e-34;

public:
    double compute(double t, const std::map<std::string, double> &params) const override
    {
        auto it_M = params.find("mass");
        auto it_r = params.find("radius");
        auto it_B = params.find("B_field");
        auto it_Bcrit = params.find("Bcrit");
        auto it_rho_fluid = params.find("rho_fluid");
        auto it_Vsys = params.find("Vsys");
        auto it_g_local = params.find("g_local");
        auto it_M_DM = params.find("M_DM");
        auto it_delta_rho = params.find("delta_rho_rho");

        double M = (it_M != params.end()) ? it_M->second : 1e30;
        double r = (it_r != params.end()) ? it_r->second : 1e4;
        double B = (it_B != params.end()) ? it_B->second : 1e10;
        double Bcrit = (it_Bcrit != params.end()) ? it_Bcrit->second : 1e11;
        double rho_fluid = (it_rho_fluid != params.end()) ? it_rho_fluid->second : 1e-15;
        double Vsys = (it_Vsys != params.end()) ? it_Vsys->second : 4.189e12;
        double g_local = (it_g_local != params.end()) ? it_g_local->second : 10.0;
        double M_DM = (it_M_DM != params.end()) ? it_M_DM->second : 0.0;
        double delta_rho_rho = (it_delta_rho != params.end()) ? it_delta_rho->second : 1e-5;

        // Base Newtonian
        double base = G * M / (r * r);

        // Expansion factor
        double H_tz = H0 * t;
        double expansion = 1 + H_tz;

        // Superconductive adjustment
        double super_adj = 1 - B / Bcrit;

        // Environment factor
        double env = 1.0;

        // Adjusted base
        double adjusted_base = base * expansion * super_adj * env;

        // Cosmological term
        double cosm = Lambda * c * c / 3.0;

        // Quantum term (simplified)
        double Delta_x_p = 1e-68;
        double integral_psi = 2.176e-18;
        double tHubble = 4.35e17;
        double quantum = (hbar / Delta_x_p) * integral_psi * (2 * M_PI / tHubble);

        // Fluid term
        double fluid = rho_fluid * Vsys * g_local;

        // Perturbation term
        double perturbation = (M + M_DM) * (delta_rho_rho + 3 * G * M / (r * r * r));

        return adjusted_base + cosm + quantum + fluid + perturbation;
    }

    std::string getName() const override { return "CompressedMUGE"; }

    std::string getDescription() const override
    {
        return "Compressed MUGE: 9-term gravity equation (base*expansion*super_adj + cosm + quantum + fluid + perturbation)";
    }

    bool validate(const std::map<std::string, double> &) const override { return true; }
};

// ============================================================================
// RESONANCE MUGE EQUATION (13 Terms + Wormhole)
// ============================================================================

class ResonanceMUGETerm : public PhysicsTerm
{
private:
    double fDPM = 1e12;
    double fTHz = 1e12;
    double Evac_neb = 7.09e-36;
    double Evac_ISM = 7.09e-37;
    double Delta_Evac = 6.381e-36;
    double Fsuper = 6.287e-19;
    double UA_SCM = 10;
    double omega_i = 1e-8;
    double k4_res = 1.0;
    double freact = 1e10;
    double fquantum = 1.445e-17;
    double fAether = 1.576e-35;
    double fTRZ = 0.1;
    double c_res = 3e8;

public:
    double compute(double t, const std::map<std::string, double> &params) const override
    {
        auto it_I = params.find("I");
        auto it_A = params.find("A");
        auto it_omega1 = params.find("omega1");
        auto it_omega2 = params.find("omega2");
        auto it_Vsys = params.find("Vsys");
        auto it_vexp = params.find("vexp");
        auto it_ffluid = params.find("ffluid");
        auto it_r = params.find("radius");

        double I = (it_I != params.end()) ? it_I->second : 1e21;
        double A = (it_A != params.end()) ? it_A->second : 3.142e8;
        double omega1 = (it_omega1 != params.end()) ? it_omega1->second : 1e-3;
        double omega2 = (it_omega2 != params.end()) ? it_omega2->second : -1e-3;
        double Vsys = (it_Vsys != params.end()) ? it_Vsys->second : 4.189e12;
        double vexp = (it_vexp != params.end()) ? it_vexp->second : 1e3;
        double ffluid = (it_ffluid != params.end()) ? it_ffluid->second : 1.269e-14;
        double r = (it_r != params.end()) ? it_r->second : 1e4;

        // 1. aDPM
        double FDPM = I * A * (omega1 - omega2);
        double aDPM = FDPM * fDPM * Evac_neb * c_res * Vsys;

        // 2. aTHz
        double aTHz = fTHz * Evac_neb * vexp * aDPM / Evac_ISM / c_res;

        // 3. avac_diff
        double avac_diff = Delta_Evac * vexp * vexp * aDPM / Evac_neb / (c_res * c_res);

        // 4. asuper_freq
        double asuper_freq = Fsuper * fTHz * aDPM / Evac_neb / c_res;

        // 5. aaether_res
        double aaether_res = UA_SCM * omega_i * fTHz * aDPM * (1 + fTRZ);

        // 6. Ug4i
        double Ereact = 1046 * std::exp(-0.0005 * t);
        double Ug4i = k4_res * Ereact * freact * aDPM / Evac_neb * c_res;

        // 7. aquantum_freq
        double aquantum_freq = fquantum * Evac_neb * aDPM / Evac_ISM / c_res;

        // 8. aAether_freq
        double aAether_freq = fAether * Evac_neb * aDPM / Evac_ISM / c_res;

        // 9. afluid_freq
        double afluid_freq = ffluid * Evac_neb * Vsys / Evac_ISM / c_res;

        // 10. Osc_term
        double Osc_term = 0.0;

        // 11. aexp_freq
        double H_z = 2.270e-18;
        double fexp = 2 * M_PI * H_z * t;
        double aexp_freq = fexp * Evac_neb * aDPM / Evac_ISM / c_res;

        // 12. fTRZ
        double fTRZ_term = fTRZ;

        // 13. Wormhole
        double b = 1.0;
        double f_worm = 1.0;
        double a_wormhole = f_worm * Evac_neb / (b * b + r * r);

        return aDPM + aTHz + avac_diff + asuper_freq + aaether_res + Ug4i +
               aquantum_freq + aAether_freq + afluid_freq + Osc_term +
               aexp_freq + fTRZ_term + a_wormhole;
    }

    std::string getName() const override { return "ResonanceMUGE"; }

    std::string getDescription() const override
    {
        return "Resonance MUGE: 13-term + wormhole resonance equation (aDPM + aTHz + avac_diff + ... + a_wormhole)";
    }

    bool validate(const std::map<std::string, double> &) const override { return true; }
};

// ============================================================================
// ASTROPHYSICAL SYSTEM TERMS (7 Systems)
// ============================================================================

class SGR1745MagnetarTerm : public PhysicsTerm
{
private:
    double I = 1e21;
    double A = 3.142e8;
    double M = 2.984e30;
    double B = 1e10;
    double z = 0.0009;

public:
    double compute(double t, const std::map<std::string, double> &params) const override
    {
        // Use ResonanceMUGE or CompressedMUGE logic with SGR1745 parameters
        std::map<std::string, double> sgr_params;
        sgr_params["I"] = I;
        sgr_params["A"] = A;
        sgr_params["mass"] = M;
        sgr_params["B_field"] = B;
        sgr_params["radius"] = 1e4;
        sgr_params["Vsys"] = 4.189e12;
        sgr_params["vexp"] = 1e3;

        ResonanceMUGETerm resonance;
        return resonance.compute(t, sgr_params);
    }

    std::string getName() const override { return "SGR1745Magnetar"; }

    std::string getDescription() const override
    {
        return "SGR 1745-2900: Magnetar system (I=1e21, M=2.984e30 kg, B=1e10 T, z=0.0009)";
    }

    bool validate(const std::map<std::string, double> &) const override { return true; }
};

class SagittariusAStarTerm : public PhysicsTerm
{
private:
    double M = 8.155e36;
    double M_DM = 1e37;
    double Vsys = 3.552e45;

public:
    double compute(double t, const std::map<std::string, double> &params) const override
    {
        std::map<std::string, double> sagA_params;
        sagA_params["mass"] = M;
        sagA_params["M_DM"] = M_DM;
        sagA_params["Vsys"] = Vsys;
        sagA_params["radius"] = 1e12;
        sagA_params["vexp"] = 5e6;
        sagA_params["I"] = 1e23;
        sagA_params["A"] = 2.813e30;

        ResonanceMUGETerm resonance;
        return resonance.compute(t, sagA_params);
    }

    std::string getName() const override { return "SagittariusAStar"; }

    std::string getDescription() const override
    {
        return "Sagittarius A*: Supermassive black hole (M=8.155e36 kg, M_DM=1e37 kg, Vsys=3.552e45 m^3)";
    }

    bool validate(const std::map<std::string, double> &) const override { return true; }
};

class TapestryStarbirthTerm : public PhysicsTerm
{
private:
    double M = 1.989e35;
    double Vsys = 1e53;

public:
    double compute(double t, const std::map<std::string, double> &params) const override
    {
        std::map<std::string, double> tapestry_params;
        tapestry_params["mass"] = M;
        tapestry_params["Vsys"] = Vsys;
        tapestry_params["radius"] = 3.086e17;
        tapestry_params["I"] = 1e22;
        tapestry_params["A"] = 1e35;

        ResonanceMUGETerm resonance;
        return resonance.compute(t, tapestry_params);
    }

    std::string getName() const override { return "TapestryStarbirth"; }

    std::string getDescription() const override
    {
        return "Tapestry of Blazing Starbirth: Nebula (M=1.989e35 kg, Vsys=1e53 m^3, r=10 pc)";
    }

    bool validate(const std::map<std::string, double> &) const override { return true; }
};

class Westerlund2ClusterTerm : public PhysicsTerm
{
public:
    double compute(double t, const std::map<std::string, double> &params) const override
    {
        std::map<std::string, double> west_params;
        west_params["mass"] = 1.989e35;
        west_params["Vsys"] = 1e53;
        west_params["radius"] = 3.086e17;
        west_params["I"] = 1e22;
        west_params["A"] = 1e35;

        ResonanceMUGETerm resonance;
        return resonance.compute(t, west_params);
    }

    std::string getName() const override { return "Westerlund2Cluster"; }

    std::string getDescription() const override
    {
        return "Westerlund 2: Stellar cluster (similar to Tapestry parameters)";
    }

    bool validate(const std::map<std::string, double> &) const override { return true; }
};

class PillarsCreationTerm : public PhysicsTerm
{
private:
    double M = 1.989e32;
    double r = 9.46e15;

public:
    double compute(double t, const std::map<std::string, double> &params) const override
    {
        std::map<std::string, double> pillars_params;
        pillars_params["mass"] = M;
        pillars_params["radius"] = r;
        pillars_params["Vsys"] = 3.552e48;
        pillars_params["I"] = 1e21;
        pillars_params["A"] = 2.813e32;

        ResonanceMUGETerm resonance;
        return resonance.compute(t, pillars_params);
    }

    std::string getName() const override { return "PillarsCreation"; }

    std::string getDescription() const override
    {
        return "Pillars of Creation: Molecular cloud (M=1.989e32 kg, r=1 ly)";
    }

    bool validate(const std::map<std::string, double> &) const override { return true; }
};

class RingsRelativityTerm : public PhysicsTerm
{
private:
    double M = 1.989e36;
    double z = 0.01;

public:
    double compute(double t, const std::map<std::string, double> &params) const override
    {
        std::map<std::string, double> rings_params;
        rings_params["mass"] = M;
        rings_params["radius"] = 3.086e17;
        rings_params["Vsys"] = 1e54;
        rings_params["vexp"] = 1e5;
        rings_params["I"] = 1e22;

        ResonanceMUGETerm resonance;
        return resonance.compute(t, rings_params);
    }

    std::string getName() const override { return "RingsRelativity"; }

    std::string getDescription() const override
    {
        return "Rings of Relativity: Cosmological structure (M=1.989e36 kg, z=0.01)";
    }

    bool validate(const std::map<std::string, double> &) const override { return true; }
};

class StudentGuideUniverseTerm : public PhysicsTerm
{
private:
    double M = 1e53;
    double r = 1e26;
    double t_Hubble = 4.35e17;

public:
    double compute(double t, const std::map<std::string, double> &params) const override
    {
        std::map<std::string, double> universe_params;
        universe_params["mass"] = M;
        universe_params["radius"] = r;
        universe_params["Vsys"] = 1e80;
        universe_params["vexp"] = 3e8;
        universe_params["I"] = 1e24;
        universe_params["A"] = 1e52;

        ResonanceMUGETerm resonance;
        return resonance.compute(t, universe_params);
    }

    std::string getName() const override { return "StudentGuideUniverse"; }

    std::string getDescription() const override
    {
        return "Student's Guide to the Universe: Observable universe (M=1e53 kg, r=10 Gly, t_Hubble=4.35e17 s)";
    }

    bool validate(const std::map<std::string, double> &) const override { return true; }
};

// ============================================================================
// HELPER TERMS
// ============================================================================

// CLASS 2: MuSTerm - Magnetic dipole moment
class MuSTerm : public PhysicsTerm
{
private:
    double Bs;          // Base magnetic field (T)
    double omega_c;     // Cyclotron frequency (rad/s)
    double Rs;          // Stellar radius (m)
    double SCm_contrib; // SCm contribution (T)

public:
    MuSTerm(double Bs_val = 1e-4, double omega_c_val = 2.7e-6, double Rs_val = 6.96e8, double SCm_val = 1e3)
        : Bs(Bs_val), omega_c(omega_c_val), Rs(Rs_val), SCm_contrib(SCm_val) {}

    double compute(double t, const std::map<std::string, double> &params) const override
    {
        double Bs_t = Bs + 0.4 * std::sin(omega_c * t) + SCm_contrib;
        return Bs_t * std::pow(Rs, 3); // A·m²
    }

    std::string getName() const override { return "MagneticDipoleMoment"; }

    std::string getDescription() const override
    {
        return "mu_s(t): Time-varying magnetic dipole moment = Bs_t * Rs^3 where Bs_t = Bs + 0.4*sin(omega_c*t) + SCm_contrib (A·m²)";
    }

    bool validate(const std::map<std::string, double> &) const override
    {
        return (Rs > 0 && Bs >= 0);
    }
};

// CLASS 3: GradMsRTerm - Surface gravity gradient
class GradMsRTerm : public PhysicsTerm
{
private:
    double Ms;                             // Stellar mass (kg)
    double Rs;                             // Stellar radius (m)
    static constexpr double G = 6.674e-11; // Gravitational constant (m³/kg·s²)

public:
    GradMsRTerm(double Ms_val = 1.989e30, double Rs_val = 6.96e8) : Ms(Ms_val), Rs(Rs_val) {}

    double compute(double t, const std::map<std::string, double> &params) const override
    {
        if (Rs == 0.0)
            throw std::runtime_error("Division by zero in Rs");
        return G * Ms / (Rs * Rs); // Surface gravity (m/s²)
    }

    std::string getName() const override { return "SurfaceGravityGradient"; }

    std::string getDescription() const override
    {
        return "grad(Ms/r): Approximate gradient of mass-to-radius ratio = G*Ms/Rs^2 (surface gravity in m/s²)";
    }

    bool validate(const std::map<std::string, double> &) const override
    {
        return (Ms > 0 && Rs > 0);
    }
};

// CLASS 4: BjTerm - Magnetic string field
class BjTerm : public PhysicsTerm
{
private:
    double omega_c;     // Cyclotron frequency (rad/s)
    double SCm_contrib; // SCm contribution (T)

public:
    BjTerm(double omega_c_val = 2.7e-6, double SCm_val = 1e3)
        : omega_c(omega_c_val), SCm_contrib(SCm_val) {}

    double compute(double t, const std::map<std::string, double> &params) const override
    {
        return 1e-3 + 0.4 * std::sin(omega_c * t) + SCm_contrib; // Tesla
    }

    std::string getName() const override { return "MagneticStringField"; }

    std::string getDescription() const override
    {
        return "Bj(t): Magnetic string field = 1e-3 + 0.4*sin(omega_c*t) + SCm_contrib (Tesla)";
    }

    bool validate(const std::map<std::string, double> &) const override
    {
        return true;
    }
};

// CLASS 5: OmegaSTTerm - Time-varying rotation frequency
class OmegaSTTerm : public PhysicsTerm
{
private:
    double omega_s; // Base stellar rotation frequency (rad/s)
    double omega_c; // Cyclotron frequency (rad/s)

public:
    OmegaSTTerm(double omega_s_val = 2.7e-6, double omega_c_val = 2.7e-6)
        : omega_s(omega_s_val), omega_c(omega_c_val) {}

    double compute(double t, const std::map<std::string, double> &params) const override
    {
        return omega_s - 0.4e-6 * std::sin(omega_c * t); // rad/s
    }

    std::string getName() const override { return "TimeVaryingRotationFrequency"; }

    std::string getDescription() const override
    {
        return "omega_s(t): Stellar rotation frequency = omega_s - 0.4e-6*sin(omega_c*t) (rad/s)";
    }

    bool validate(const std::map<std::string, double> &) const override
    {
        return true;
    }
};

// CLASS 6: MuJTerm - Magnetic string dipole moment
class MuJTerm : public PhysicsTerm
{
private:
    double omega_c;     // Cyclotron frequency (rad/s)
    double Rs;          // Stellar radius (m)
    double SCm_contrib; // SCm contribution (T)

public:
    MuJTerm(double omega_c_val = 2.7e-6, double Rs_val = 6.96e8, double SCm_val = 1e3)
        : omega_c(omega_c_val), Rs(Rs_val), SCm_contrib(SCm_val) {}

    double compute(double t, const std::map<std::string, double> &params) const override
    {
        // Call Bj computation inline
        double Bj = 1e-3 + 0.4 * std::sin(omega_c * t) + SCm_contrib;
        return Bj * std::pow(Rs, 3); // A·m²
    }

    std::string getName() const override { return "StringDipoleMoment"; }

    std::string getDescription() const override
    {
        return "mu_j(t): Magnetic string dipole moment = Bj(t) * Rs^3 (A·m²)";
    }

    bool validate(const std::map<std::string, double> &) const override
    {
        return (Rs > 0);
    }
};

// CLASS 1: ReactorEfficiencyTerm (already exists)
class ReactorEfficiencyTerm : public PhysicsTerm
{
private:
    double kappa = 0.0005;

public:
    double compute(double t, const std::map<std::string, double> &params) const override
    {
        auto it_rho_SCm = params.find("rho_SCm");
        auto it_v_SCm = params.find("v_SCm");
        auto it_rho_A = params.find("rho_A");

        double rho_SCm = (it_rho_SCm != params.end()) ? it_rho_SCm->second : 1e15;
        double v_SCm = (it_v_SCm != params.end()) ? it_v_SCm->second : 0.99 * 3e8;
        double rho_A = (it_rho_A != params.end()) ? it_rho_A->second : 1e-23;

        return (rho_SCm * v_SCm * v_SCm / rho_A) * std::exp(-kappa * t);
    }

    std::string getName() const override { return "ReactorEfficiency"; }

    std::string getDescription() const override
    {
        return "Ereact: SCm reactor efficiency (rho_SCm*v_SCm^2/rho_A*exp(-kappa*t))";
    }

    bool validate(const std::map<std::string, double> &) const override { return true; }
};

class NavierStokesQuasarJetTerm : public PhysicsTerm
{
private:
    double visc = 0.0001;
    double dt_ns = 0.1;

public:
    double compute(double t, const std::map<std::string, double> &params) const override
    {
        auto it_uqff_g = params.find("uqff_g");
        auto it_v_jet = params.find("v_jet");

        double uqff_g = (it_uqff_g != params.end()) ? it_uqff_g->second : 0.0;
        double v_jet = (it_v_jet != params.end()) ? it_v_jet->second : 0.99 * 3e8;

        // Simplified: return velocity change from UQFF body force
        return dt_ns * uqff_g + v_jet / 1e10;
    }

    std::string getName() const override { return "NavierStokesQuasarJet"; }

    std::string getDescription() const override
    {
        return "NS Quasar Jet: Navier-Stokes with UQFF body force (v += dt*uqff_g, v_jet=0.99c)";
    }

    bool validate(const std::map<std::string, double> &) const override { return true; }
};

// ============================================================================
// REGISTRATION FUNCTION
// ============================================================================

void registerWolframTerms_source4(PhysicsTermRegistry &registry)
{
    // Universal Gravity (4 terms)
    registry.registerPhysicsTerm("UniversalGravity1", std::make_unique<UniversalGravity1Term>(), "wolfram");
    registry.registerPhysicsTerm("UniversalGravity2", std::make_unique<UniversalGravity2Term>(), "wolfram");
    registry.registerPhysicsTerm("UniversalGravity3", std::make_unique<UniversalGravity3Term>(), "wolfram");
    registry.registerPhysicsTerm("UniversalGravity4", std::make_unique<UniversalGravity4Term>(), "wolfram");

    // Universal Buoyancy, Magnetism, Aether (3 terms)
    registry.registerPhysicsTerm("UniversalBuoyancy", std::make_unique<UniversalBuoyancyTerm>(), "wolfram");
    registry.registerPhysicsTerm("UniversalMagnetism", std::make_unique<UniversalMagnetismTerm>(), "wolfram");
    registry.registerPhysicsTerm("UniversalAether", std::make_unique<UniversalAetherTerm>(), "wolfram");

    // Unified Field (1 term)
    registry.registerPhysicsTerm("UnifiedField", std::make_unique<UnifiedFieldTerm>(), "wolfram");

    // MUGE Equations (2 terms)
    registry.registerPhysicsTerm("CompressedMUGE", std::make_unique<CompressedMUGETerm>(), "wolfram");
    registry.registerPhysicsTerm("ResonanceMUGE", std::make_unique<ResonanceMUGETerm>(), "wolfram");

    // Astrophysical Systems (7 terms)
    registry.registerPhysicsTerm("SGR1745Magnetar", std::make_unique<SGR1745MagnetarTerm>(), "wolfram");
    registry.registerPhysicsTerm("SagittariusAStar", std::make_unique<SagittariusAStarTerm>(), "wolfram");
    registry.registerPhysicsTerm("TapestryStarbirth", std::make_unique<TapestryStarbirthTerm>(), "wolfram");
    registry.registerPhysicsTerm("Westerlund2Cluster", std::make_unique<Westerlund2ClusterTerm>(), "wolfram");
    registry.registerPhysicsTerm("PillarsCreation", std::make_unique<PillarsCreationTerm>(), "wolfram");
    registry.registerPhysicsTerm("RingsRelativity", std::make_unique<RingsRelativityTerm>(), "wolfram");
    registry.registerPhysicsTerm("StudentGuideUniverse", std::make_unique<StudentGuideUniverseTerm>(), "wolfram");

    // Helper Terms (7 terms - 5 NEW + 2 existing)
    registry.registerPhysicsTerm("MagneticDipoleMoment", std::make_unique<MuSTerm>(), "wolfram");
    registry.registerPhysicsTerm("SurfaceGravityGradient", std::make_unique<GradMsRTerm>(), "wolfram");
    registry.registerPhysicsTerm("MagneticStringField", std::make_unique<BjTerm>(), "wolfram");
    registry.registerPhysicsTerm("TimeVaryingRotationFrequency", std::make_unique<OmegaSTTerm>(), "wolfram");
    registry.registerPhysicsTerm("StringDipoleMoment", std::make_unique<MuJTerm>(), "wolfram");
    registry.registerPhysicsTerm("ReactorEfficiency", std::make_unique<ReactorEfficiencyTerm>(), "wolfram");
    registry.registerPhysicsTerm("NavierStokesQuasarJet", std::make_unique<NavierStokesQuasarJetTerm>(), "wolfram");
}
