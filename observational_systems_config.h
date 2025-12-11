// observational_systems_config.h
// System parameter configurations for observational astrophysical systems
// Use with existing SOURCE10 buoyancy physics terms in MAIN_1_CoAnQi.cpp
// Copyright - Daniel T. Murphy, 2025

#ifndef OBSERVATIONAL_SYSTEMS_CONFIG_H
#define OBSERVATIONAL_SYSTEMS_CONFIG_H

#include <map>
#include <string>

struct ObservationalSystem
{
    std::string name;
    std::string description;

    // Physical parameters
    double M;       // Mass (kg)
    double r;       // Radius (m)
    double L_X;     // X-ray luminosity (W)
    double B0;      // Magnetic field (T)
    double rho_gas; // Gas density (kg/m³)
    double T_gas;   // Gas temperature (K)
    double omega0;  // Angular frequency (rad/s)
    double t_age;   // System age/timescale (s)

    // Observational context
    std::string category;  // "galaxy_cluster", "pulsar", "agn", "nebula", "tde", "snr"
    std::string telescope; // Primary observation source
};

// ============================================================================
// SYSTEM DEFINITIONS (35+ observational targets from Source155-161)
// ============================================================================

static const std::map<std::string, ObservationalSystem> OBSERVATIONAL_SYSTEMS = {

    // ========== GALAXY CLUSTERS (Source155, 156, 157, 158, 159, 160) ==========

    {"ESO137", {"ESO 137-001", "Ram pressure stripping galaxy in Abell 3627 cluster",
                2e41,    // M (kg)
                6.17e21, // r (m)
                1e34,    // L_X (W)
                2e-9,    // B0 (T)
                1e-23,   // rho_gas (kg/m³)
                1e7,     // T_gas (K)
                1e-15,   // omega0 (rad/s)
                7.72e14, // t_age (s, ~24.5 Myr)
                "galaxy_cluster", "Chandra/ALMA/MUSE"}},

    {"NGC1365", {"NGC 1365", "Barred spiral galaxy with active nucleus",
                 7.17e41, // M (kg)
                 9.46e20, // r (m)
                 1e36,    // L_X (W)
                 1e-9,    // B0 (T)
                 1e-23,   // rho_gas (kg/m³)
                 2e7,     // T_gas (K)
                 1e-15,   // omega0 (rad/s)
                 1.1e16,  // t_age (s, ~350 Myr)
                 "agn", "Chandra/JWST"}},

    {"ElGordo", {"El Gordo (ACT-CL J0102-4915)", "Most massive distant galaxy cluster merger",
                 4.97e45, // M (kg)
                 3.09e22, // r (m)
                 2e38,    // L_X (W)
                 1e-10,   // B0 (T)
                 1e-24,   // rho_gas (kg/m³)
                 1.4e8,   // T_gas (K)
                 1e-15,   // omega0 (rad/s)
                 2.21e16, // t_age (s, ~700 Myr)
                 "galaxy_cluster", "Chandra/ALMA/VLT"}},

    {"PLCKG287", {"PLCK G287.0+32.9", "Planck-detected galaxy cluster",
                  1.989e44, // M (kg)
                  3.09e22,  // r (m)
                  5e37,     // L_X (W)
                  5e-10,    // B0 (T)
                  5e-24,    // rho_gas (kg/m³)
                  8e7,      // T_gas (K)
                  1e-15,    // omega0 (rad/s)
                  1.5e16,   // t_age (s)
                  "galaxy_cluster", "Planck/Chandra"}},

    {"PSZ2G181", {"PSZ2 G181.06+48.47", "Planck SZ2 catalog galaxy cluster",
                  1.989e44, // M (kg)
                  3.09e22,  // r (m)
                  4e37,     // L_X (W)
                  4e-10,    // B0 (T)
                  4e-24,    // rho_gas (kg/m³)
                  7e7,      // T_gas (K)
                  1e-15,    // omega0 (rad/s)
                  1.8e16,   // t_age (s)
                  "galaxy_cluster", "Planck/XMM"}},

    {"NGC4839", {"NGC 4839", "Galaxy in Coma Cluster showing ram pressure effects",
                 3e41,   // M (kg)
                 8e20,   // r (m)
                 8e35,   // L_X (W)
                 1.5e-9, // B0 (T)
                 2e-23,  // rho_gas (kg/m³)
                 1.5e7,  // T_gas (K)
                 1e-15,  // omega0 (rad/s)
                 1e16,   // t_age (s)
                 "galaxy_cluster", "Chandra/HST"}},

    {"Abell2256", {"Abell 2256", "Merging galaxy cluster with radio relics",
                   1.23e45, // M (kg)
                   3.93e22, // r (m)
                   1.5e38,  // L_X (W)
                   2e-10,   // B0 (T)
                   1e-24,   // rho_gas (kg/m³)
                   9e7,     // T_gas (K)
                   1e-15,   // omega0 (rad/s)
                   2e16,    // t_age (s)
                   "galaxy_cluster", "Chandra/VLA"}},

    {"M84", {"M84 (NGC 4374)", "Lenticular galaxy in Virgo Cluster with jets",
             1.46e45, // M (kg)
             3.09e22, // r (m)
             3e37,    // L_X (W)
             8e-10,   // B0 (T)
             3e-24,   // rho_gas (kg/m³)
             6e7,     // T_gas (K)
             1e-15,   // omega0 (rad/s)
             1.2e16,  // t_age (s)
             "agn", "Chandra/HST/VLA"}},

    // ========== PULSARS (Source155, 156, 161) ==========

    {"Vela", {"Vela Pulsar", "Young pulsar with pulsar wind nebula",
              2.8e30,  // M (kg, ~1.4 solar masses)
              1.7e17,  // r (m, ~170 Gm standoff distance)
              1e27,    // L_X (W)
              3e-8,    // B0 (T, surface field ~1e8 G)
              1e-23,   // rho_gas (kg/m³)
              1e6,     // T_gas (K, PWN)
              1e-12,   // omega0 (rad/s, ~89 ms period)
              3.47e11, // t_age (s, ~11,000 years)
              "pulsar", "Chandra/Fermi"}},

    {"J1610", {"J1610+1811", "Millisecond pulsar in globular cluster",
               2.785e30, // M (kg)
               3.09e15,  // r (m, light cylinder radius)
               5e26,     // L_X (W)
               1e-8,     // B0 (T)
               1e-22,    // rho_gas (kg/m³)
               5e5,      // T_gas (K)
               1e-11,    // omega0 (rad/s)
               1e15,     // t_age (s, old recycled)
               "pulsar", "Chandra/XMM"}},

    {"Crab", {"Crab Nebula Pulsar", "Young energetic pulsar with wind nebula",
              1e31,    // M (kg, includes nebula mass)
              4.73e16, // r (m, nebula radius)
              1e31,    // L_X (W, total nebula)
              5e-8,    // B0 (T, pulsar surface ~1e12 G)
              1e-22,   // rho_gas (kg/m³)
              1e7,     // T_gas (K)
              2e-10,   // omega0 (rad/s, 33 ms)
              2.9e10,  // t_age (s, ~900 years)
              "pulsar", "Chandra/HST/JWST"}},

    {"ASKAPJ1832", {"ASKAP J1832-0911", "Radio pulsar discovered by ASKAP",
                    2.785e30, // M (kg)
                    4.63e16,  // r (m)
                    3e26,     // L_X (W)
                    2e-8,     // B0 (T)
                    1e-23,    // rho_gas (kg/m³)
                    8e5,      // T_gas (K)
                    1e-12,    // omega0 (rad/s)
                    5e14,     // t_age (s)
                    "pulsar", "ASKAP/Parkes"}},

    // ========== ACTIVE GALACTIC NUCLEI (Source155, 157, 158) ==========

    {"CentaurusA", {"Centaurus A (NGC 5128)", "Nearest radio galaxy with prominent jets",
                    1.094e38, // M (kg, SMBH mass ~5.5e7 solar)
                    6.17e17,  // r (m, jet interaction region)
                    2e34,     // L_X (W)
                    1e-6,     // B0 (T, jet magnetic field)
                    1e-21,    // rho_gas (kg/m³)
                    5e6,      // T_gas (K)
                    1e-12,    // omega0 (rad/s)
                    1e14,     // t_age (s)
                    "agn", "Chandra/ALMA/VLT"}},

    {"M104", {"M104 (Sombrero Galaxy)", "Edge-on spiral with large bulge and SMBH",
              1.5e38, // M (kg, SMBH ~8e8 solar)
              5e20,   // r (m)
              5e33,   // L_X (W)
              5e-10,  // B0 (T)
              5e-24,  // rho_gas (kg/m³)
              3e6,    // T_gas (K)
              1e-13,  // omega0 (rad/s)
              2e16,   // t_age (s)
              "agn", "Chandra/HST"}},

    // ========== TIDAL DISRUPTION EVENT (Source155) ==========

    {"ASASSN14li", {"ASASSN-14li", "Tidal disruption event with jetted outflow",
                    1.989e37, // M (kg, SMBH ~3e6 solar)
                    3.09e18,  // r (m, tidal radius)
                    1e37,     // L_X (W, peak)
                    1e-5,     // B0 (T, accretion disk field)
                    1e-21,    // rho_gas (kg/m³, debris)
                    1e7,      // T_gas (K)
                    1e-12,    // omega0 (rad/s)
                    9.504e6,  // t_age (s, ~110 days since disruption)
                    "tde", "Chandra/Swift/ASAS-SN"}},

    // ========== NEBULAE (Source157, 158, 159, 160) ==========

    {"NGC346", {"NGC 346", "Star-forming region in Small Magellanic Cloud",
                1e36,   // M (kg)
                1.5e17, // r (m, ~50 pc)
                2e32,   // L_X (W)
                1e-9,   // B0 (T)
                1e-20,  // rho_gas (kg/m³)
                1e4,    // T_gas (K, HII region)
                1e-14,  // omega0 (rad/s)
                1e14,   // t_age (s, ~3 Myr)
                "nebula", "HST/JWST/Chandra"}},

    {"M16", {"M16 (Eagle Nebula)", "Star-forming region with Pillars of Creation",
             1e36,    // M (kg)
             2.36e17, // r (m)
             5e32,    // L_X (W)
             2e-9,    // B0 (T)
             1e-20,   // rho_gas (kg/m³)
             8e3,     // T_gas (K)
             1e-14,   // omega0 (rad/s)
             2e14,    // t_age (s, ~5.5 Myr)
             "nebula", "HST/Spitzer/Chandra"}},

    {"NGC1672", {"NGC 1672", "Barred spiral with intense star formation",
                 5e41,   // M (kg)
                 7e20,   // r (m)
                 2e36,   // L_X (W)
                 1.2e-9, // B0 (T)
                 1e-23,  // rho_gas (kg/m³)
                 1.5e7,  // T_gas (K)
                 1e-15,  // omega0 (rad/s)
                 1.5e16, // t_age (s)
                 "nebula", "HST/Chandra"}},

    {"Tarantula", {"Tarantula Nebula (30 Doradus)", "Giant HII region in Large Magellanic Cloud",
                   1e36,  // M (kg)
                   2e17,  // r (m, ~65 pc)
                   1e33,  // L_X (W)
                   3e-9,  // B0 (T)
                   1e-20, // rho_gas (kg/m³)
                   1e4,   // T_gas (K)
                   1e-14, // omega0 (rad/s)
                   3e14,  // t_age (s, ~8 Myr)
                   "nebula", "HST/Spitzer/Chandra"}},

    // ========== SUPERNOVA REMNANTS (Source158, 159, 160) ==========

    {"Tycho", {"Tycho's Supernova Remnant", "Type Ia supernova remnant from 1572",
               1e31,    // M (kg, ejecta + swept-up)
               1e17,    // r (m, ~3 pc)
               5e30,    // L_X (W)
               1e-7,    // B0 (T, shock magnetic field)
               1e-22,   // rho_gas (kg/m³)
               2e7,     // T_gas (K, shock-heated)
               1e-13,   // omega0 (rad/s)
               1.43e10, // t_age (s, ~453 years)
               "snr", "Chandra/XMM/VLA"}},

    // ========== GALAXIES (Source157, 158, 160) ==========

    {"M74", {"M74 (NGC 628)", "Grand design spiral galaxy",
             7.17e41, // M (kg)
             9.46e20, // r (m)
             5e35,    // L_X (W)
             8e-10,   // B0 (T)
             1e-23,   // rho_gas (kg/m³)
             1e7,     // T_gas (K)
             1e-15,   // omega0 (rad/s)
             2e16,    // t_age (s)
             "galaxy", "HST/Chandra/JWST"}},

    {"NGC253", {"NGC 253 (Sculptor Galaxy)", "Starburst galaxy with superwind",
                4e40,  // M (kg)
                4e20,  // r (m)
                1e36,  // L_X (W)
                2e-9,  // B0 (T)
                2e-23, // rho_gas (kg/m³)
                2e7,   // T_gas (K, hot wind)
                1e-15, // omega0 (rad/s)
                1e16,  // t_age (s)
                "galaxy", "Chandra/ALMA/VLT"}},

    // ========== MULTI-WAVELENGTH COLLECTIONS (Source156, 161) ==========

    {"Sonification", {"Chandra Sonification Collection", "Multi-system sonified X-ray data collection",
                      1.989e31, // M (kg, representative)
                      6.17e16,  // r (m, representative)
                      1e32,     // L_X (W, representative)
                      1e-9,     // B0 (T, representative)
                      1e-22,    // rho_gas (kg/m³)
                      1e6,      // T_gas (K)
                      1e-13,    // omega0 (rad/s)
                      1e15,     // t_age (s)
                      "multi_system", "Chandra"}},

    {"ChandraWebb", {"Chandra-Webb Collaborative Observations", "Combined X-ray and infrared multi-system survey",
                     1e40,  // M (kg, representative)
                     1e20,  // r (m, representative)
                     1e35,  // L_X (W, representative)
                     1e-9,  // B0 (T, representative)
                     1e-23, // rho_gas (kg/m³)
                     1e7,   // T_gas (K)
                     1e-14, // omega0 (rad/s)
                     1e16,  // t_age (s)
                     "multi_system", "Chandra/JWST"}},

    {"SupernovaSurvey", {"Supernova Observational Survey", "Multi-supernova comparative study",
                         1e30,  // M (kg, representative NS)
                         1e10,  // r (m, representative remnant)
                         1e31,  // L_X (W, representative)
                         1e-7,  // B0 (T, representative)
                         1e-22, // rho_gas (kg/m³)
                         1e7,   // T_gas (K)
                         1e-13, // omega0 (rad/s)
                         1e11,  // t_age (s, representative ~3000 yrs)
                         "multi_system", "Chandra/HST/Swift"}}};

// ============================================================================
// HELPER FUNCTIONS
// ============================================================================

// Get system parameters by name
inline const ObservationalSystem *getSystem(const std::string &name)
{
    auto it = OBSERVATIONAL_SYSTEMS.find(name);
    if (it != OBSERVATIONAL_SYSTEMS.end())
    {
        return &(it->second);
    }
    return nullptr;
}

// Convert system to parameter map for use with PhysicsTerm::compute()
inline std::map<std::string, double> systemToParams(const std::string &system_name)
{
    const ObservationalSystem *sys = getSystem(system_name);
    if (!sys)
        return {};

    std::map<std::string, double> params;
    params["M"] = sys->M;
    params["r"] = sys->r;
    params["L_X"] = sys->L_X;
    params["B0"] = sys->B0;
    params["rho_gas"] = sys->rho_gas;
    params["T_gas"] = sys->T_gas;
    params["omega0"] = sys->omega0;
    params["t_age"] = sys->t_age;

    // Add universal constants
    params["G"] = 6.6743e-11;
    params["c"] = 3e8;
    params["hbar"] = 1.0546e-34;
    params["k_B"] = 1.38e-23;
    params["m_e"] = 9.11e-31;

    return params;
}

// List all available systems
inline std::vector<std::string> listSystems()
{
    std::vector<std::string> names;
    for (const auto &pair : OBSERVATIONAL_SYSTEMS)
    {
        names.push_back(pair.first);
    }
    return names;
}

// Get systems by category
inline std::vector<std::string> getSystemsByCategory(const std::string &category)
{
    std::vector<std::string> names;
    for (const auto &pair : OBSERVATIONAL_SYSTEMS)
    {
        if (pair.second.category == category)
        {
            names.push_back(pair.first);
        }
    }
    return names;
}

#endif // OBSERVATIONAL_SYSTEMS_CONFIG_H
