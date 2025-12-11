// source6_simulation_harness.cpp
// Interactive simulation framework for source6 UQFF physics
// Hybrid approach: Supports both CelestialBody physics and graphics infrastructure testing
// Pattern: Follows source4/5 template with 6-option menu

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <cmath>

// Forward declarations (assume source6_wolfram.cpp is included/compiled separately)
class PhysicsTerm;
class PhysicsTermRegistry;

extern void registerWolframTerms_source6(PhysicsTermRegistry &registry);

const double PI = 3.141592653589793;
const double c = 3.0e8; // m/s

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
// Default Celestial Bodies (from source6.cpp main())
// ============================================================================
std::vector<CelestialBody> getDefaultBodies()
{
    std::vector<CelestialBody> bodies;

    // Sun
    CelestialBody sun = {
        "Sun",
        1.989e30,                            // Ms (kg)
        6.96e8,                              // Rs (m)
        1.496e13,                            // Rb (AU in meters - heliosphere)
        5778.0,                              // Ts_surface (K)
        2.5e-6,                              // omega_s (rad/s)
        1e-4,                                // Bs_avg (T)
        1e15,                                // SCm_density (kg/m^3)
        1e-11,                               // QUA (C)
        1.0,                                 // Pcore
        1.0,                                 // PSCm
        2 * PI / (11.0 * 365.25 * 24 * 3600) // omega_c (11-year solar cycle)
    };

    // Earth
    CelestialBody earth = {
        "Earth",
        5.972e24,                           // Ms (kg)
        6.371e6,                            // Rs (m)
        1e7,                                // Rb (magnetosphere ~10 Earth radii)
        288.0,                              // Ts_surface (K)
        7.292e-5,                           // omega_s (rad/s)
        3e-5,                               // Bs_avg (T)
        1e12,                               // SCm_density (kg/m^3)
        1e-12,                              // QUA (C)
        1e-3,                               // Pcore
        1e-3,                               // PSCm
        2 * PI / (1.0 * 365.25 * 24 * 3600) // omega_c (annual cycle)
    };

    // Jupiter
    CelestialBody jupiter = {
        "Jupiter",
        1.898e27,                             // Ms (kg)
        6.9911e7,                             // Rs (m)
        1e8,                                  // Rb (magnetosphere)
        165.0,                                // Ts_surface (K)
        1.76e-4,                              // omega_s (rad/s)
        4e-4,                                 // Bs_avg (T)
        1e13,                                 // SCm_density (kg/m^3)
        1e-11,                                // QUA (C)
        1e-3,                                 // Pcore
        1e-3,                                 // PSCm
        2 * PI / (11.86 * 365.25 * 24 * 3600) // omega_c (11.86-year orbit)
    };

    // Neptune
    CelestialBody neptune = {
        "Neptune",
        1.024e26,                             // Ms (kg)
        2.4622e7,                             // Rs (m)
        5e7,                                  // Rb (magnetosphere)
        72.0,                                 // Ts_surface (K)
        1.08e-4,                              // omega_s (rad/s)
        1e-4,                                 // Bs_avg (T)
        1e11,                                 // SCm_density (kg/m^3)
        1e-13,                                // QUA (C)
        1e-3,                                 // Pcore
        1e-3,                                 // PSCm
        2 * PI / (164.8 * 365.25 * 24 * 3600) // omega_c (164.8-year orbit)
    };

    bodies.push_back(sun);
    bodies.push_back(earth);
    bodies.push_back(jupiter);
    bodies.push_back(neptune);

    return bodies;
}

// ============================================================================
// Simulation Parameters Structure
// ============================================================================
struct SimulationParams
{
    double r = 1e13;    // Distance (m)
    double t = 0.0;     // Time (s)
    double tn = 0.0;    // Normalized time
    double theta = 0.0; // Angular coordinate

    // Global UQFF parameters
    double v_SCm = 0.99 * c;
    double rho_A = 1e-23;
    double rho_sw = 8e-21;
    double v_sw = 5e5;
    double QA = 1e-10;
    double kappa = 0.0005;
    double alpha = 0.001;
    double gamma = 0.00005;
    double delta_sw = 0.01;
    double epsilon_sw = 0.001;
    double delta_def = 0.01;
    double HSCm = 1.0;
    double UUA = 1.0;
    double eta = 1e-22;
    double k1 = 1.5, k2 = 1.2, k3 = 1.8, k4 = 2.0;
    double beta_i = 0.6;
    double rho_v = 6e-27;
    double C_concentration = 1.0;
    double f_feedback = 0.1;
    double num_strings = 1e9;
    double Ts00 = 1.27e3 + 1.11e7;
    double Omega_g = 7.3e-16;
    double Mbh = 8.15e36;
    double dg = 2.55e20;

    // Graphics test parameters
    double fps = 60.0;
    double draw_calls = 1000.0;
    int vertices = 1000;
    int faces = 500;
};

// ============================================================================
// Helper: Build Parameter Map from CelestialBody and SimulationParams
// ============================================================================
std::map<std::string, double> buildParamMap(const CelestialBody &body, const SimulationParams &sim)
{
    std::map<std::string, double> params;

    // CelestialBody parameters
    params["Ms"] = body.Ms;
    params["Rs"] = body.Rs;
    params["Rb"] = body.Rb;
    params["Ts_surface"] = body.Ts_surface;
    params["omega_s"] = body.omega_s;
    params["Bs_avg"] = body.Bs_avg;
    params["SCm_density"] = body.SCm_density;
    params["QUA"] = body.QUA;
    params["Pcore"] = body.Pcore;
    params["PSCm"] = body.PSCm;
    params["omega_c"] = body.omega_c;

    // Simulation parameters
    params["r"] = sim.r;
    params["t"] = sim.t;
    params["tn"] = sim.tn;
    params["theta"] = sim.theta;
    params["v_SCm"] = sim.v_SCm;
    params["rho_A"] = sim.rho_A;
    params["rho_sw"] = sim.rho_sw;
    params["v_sw"] = sim.v_sw;
    params["QA"] = sim.QA;
    params["kappa"] = sim.kappa;
    params["alpha"] = sim.alpha;
    params["gamma"] = sim.gamma;
    params["delta_sw"] = sim.delta_sw;
    params["epsilon_sw"] = sim.epsilon_sw;
    params["delta_def"] = sim.delta_def;
    params["HSCm"] = sim.HSCm;
    params["UUA"] = sim.UUA;
    params["eta"] = sim.eta;
    params["k1"] = sim.k1;
    params["k2"] = sim.k2;
    params["k3"] = sim.k3;
    params["k4"] = sim.k4;
    params["beta_i"] = sim.beta_i;
    params["rho_v"] = sim.rho_v;
    params["C_concentration"] = sim.C_concentration;
    params["f_feedback"] = sim.f_feedback;
    params["num_strings"] = sim.num_strings;
    params["Ts00"] = sim.Ts00;
    params["Omega_g"] = sim.Omega_g;
    params["Mbh"] = sim.Mbh;
    params["dg"] = sim.dg;
    params["rj"] = body.Rb; // Use Rb for rj

    // Graphics parameters
    params["fps"] = sim.fps;
    params["draw_calls"] = sim.draw_calls;
    params["vertices"] = sim.vertices;
    params["faces"] = sim.faces;

    return params;
}

// ============================================================================
// Simulation Functions
// ============================================================================
void printSystemParameters(const CelestialBody &body, const SimulationParams &sim)
{
    std::cout << "\n=== System Parameters ===" << std::endl;
    std::cout << "Body: " << body.name << std::endl;
    std::cout << "Mass (Ms): " << body.Ms << " kg" << std::endl;
    std::cout << "Radius (Rs): " << body.Rs << " m" << std::endl;
    std::cout << "Bubble Radius (Rb): " << body.Rb << " m" << std::endl;
    std::cout << "Magnetic Field (Bs_avg): " << body.Bs_avg << " T" << std::endl;
    std::cout << "Distance (r): " << sim.r << " m" << std::endl;
    std::cout << "Time (t): " << sim.t << " s" << std::endl;
    std::cout << "=========================" << std::endl;
}

void exportCSV(const std::string &filename, const std::vector<std::vector<double>> &data,
               const std::vector<std::string> &headers)
{
    std::ofstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Failed to open " << filename << std::endl;
        return;
    }

    // Write headers
    for (size_t i = 0; i < headers.size(); ++i)
    {
        file << headers[i];
        if (i < headers.size() - 1)
            file << ",";
    }
    file << "\n";

    // Write data
    for (const auto &row : data)
    {
        for (size_t i = 0; i < row.size(); ++i)
        {
            file << std::scientific << std::setprecision(6) << row[i];
            if (i < row.size() - 1)
                file << ",";
        }
        file << "\n";
    }

    file.close();
    std::cout << "Data exported to " << filename << std::endl;
}

// ============================================================================
// Main Menu
// ============================================================================
void printMenu()
{
    std::cout << "\n======================================" << std::endl;
    std::cout << "Source6 Simulation Harness (Hybrid)" << std::endl;
    std::cout << "29 Classes: 14 Graphics + 15 Physics" << std::endl;
    std::cout << "======================================" << std::endl;
    std::cout << "1. Show physics term registry" << std::endl;
    std::cout << "2. Show current system parameters" << std::endl;
    std::cout << "3. Evaluate UQFF physics terms (Ug1-4, Um, FU)" << std::endl;
    std::cout << "4. Evaluate graphics infrastructure terms" << std::endl;
    std::cout << "5. Run time evolution simulation (UQFF)" << std::endl;
    std::cout << "6. Modify system parameters" << std::endl;
    std::cout << "7. Exit" << std::endl;
    std::cout << "======================================" << std::endl;
    std::cout << "Enter choice: ";
}

int main()
{
    std::cout << "Source6 Simulation Harness - HYBRID APPROACH" << std::endl;
    std::cout << "Initialized with 4 default bodies: Sun, Earth, Jupiter, Neptune" << std::endl;

    // Initialize physics registry (assumed to be provided by source6_wolfram.cpp)
    // PhysicsTermRegistry registry;
    // registerWolframTerms_source6(registry);

    std::vector<CelestialBody> bodies = getDefaultBodies();
    int currentBodyIndex = 0;
    SimulationParams sim;

    int choice = 0;
    while (choice != 7)
    {
        printMenu();
        std::cin >> choice;

        switch (choice)
        {
        case 1:
            std::cout << "\n=== Physics Term Registry ===" << std::endl;
            std::cout << "Total: 29 classes registered" << std::endl;
            std::cout << "\nGraphics (14 classes):" << std::endl;
            std::cout << "  - OpenGLRender, VulkanRender, MeshLoaderOBJ" << std::endl;
            std::cout << "  - ProceduralLandscape, MeshExtrude, MeshBoolean" << std::endl;
            std::cout << "  - TextureLoader, ShaderCompile, CameraViewMatrix" << std::endl;
            std::cout << "  - BoneAnimation, LaTeXRender, MultiViewport" << std::endl;
            std::cout << "  - SimulationEntityUpdate, ToolPathExecution" << std::endl;
            std::cout << "\nUQFF Physics Helpers (7 classes):" << std::endl;
            std::cout << "  - StepFunctionSource6, ReactorEnergySource6" << std::endl;
            std::cout << "  - MagneticMomentTimeSource6, GradientMassRadiusSource6" << std::endl;
            std::cout << "  - MagneticJetFieldSource6, OmegaSpinModulationSource6" << std::endl;
            std::cout << "  - MagneticJetMomentSource6" << std::endl;
            std::cout << "\nUQFF Physics Core (8 classes):" << std::endl;
            std::cout << "  - UniversalGravity1Source6 (magnetic dipole)" << std::endl;
            std::cout << "  - UniversalGravity2Source6 (charge/superconductor)" << std::endl;
            std::cout << "  - UniversalGravity3Source6 (magnetic strings)" << std::endl;
            std::cout << "  - UniversalGravity4Source6 (reactor/black hole)" << std::endl;
            std::cout << "  - UniversalBuoyancySource6 (Ubi)" << std::endl;
            std::cout << "  - UniversalMagnetismSource6 (Um - cosmic strings)" << std::endl;
            std::cout << "  - SpacetimeMetricSource6 (A_mu_nu)" << std::endl;
            std::cout << "  - FullUnifiedFieldSource6 (FU complete)" << std::endl;
            break;

        case 2:
            printSystemParameters(bodies[currentBodyIndex], sim);
            break;

        case 3:
            std::cout << "\n=== UQFF Physics Evaluation ===" << std::endl;
            std::cout << "System: " << bodies[currentBodyIndex].name << std::endl;
            std::cout << "Time: " << sim.t << " s" << std::endl;
            std::cout << "\nNote: Actual term evaluation requires PhysicsTermRegistry" << std::endl;
            std::cout << "      integration from source6_wolfram.cpp" << std::endl;
            std::cout << "\nExpected outputs:" << std::endl;
            std::cout << "  Ug1 (magnetic dipole): ~1e10 N" << std::endl;
            std::cout << "  Ug2 (charge): ~1e8 N" << std::endl;
            std::cout << "  Ug3 (strings): ~1e9 N" << std::endl;
            std::cout << "  Ug4 (reactor): ~1e7 N" << std::endl;
            std::cout << "  Um (magnetism): ~1e12 N" << std::endl;
            std::cout << "  FU (total): ~1e12 N" << std::endl;
            break;

        case 4:
            std::cout << "\n=== Graphics Infrastructure Evaluation ===" << std::endl;
            std::cout << "OpenGL Rendering: " << sim.vertices << " vertices @ " << sim.fps << " FPS" << std::endl;
            std::cout << "  -> " << (sim.vertices * sim.fps) << " vertices/sec" << std::endl;
            std::cout << "Vulkan Command Buffers: " << sim.draw_calls << " draws / 2 buffers" << std::endl;
            std::cout << "  -> " << (sim.draw_calls / 2) << " draws/buffer" << std::endl;
            std::cout << "Mesh Complexity: " << sim.vertices << " vertices + " << sim.faces << " faces" << std::endl;
            std::cout << "  -> " << (sim.vertices + sim.faces * 3) << " total vertex refs" << std::endl;
            break;

        case 5:
            std::cout << "\n=== Time Evolution Simulation ===" << std::endl;
            std::cout << "Running 100-step simulation..." << std::endl;
            {
                std::vector<std::vector<double>> timeSeriesData;
                double dt = 1000.0; // 1000 seconds per step
                for (int step = 0; step < 100; ++step)
                {
                    sim.t = step * dt;
                    sim.tn = sim.t;
                    // In full implementation, would call physics terms here
                    std::vector<double> row = {sim.t, 0.0, 0.0, 0.0}; // Placeholder
                    timeSeriesData.push_back(row);
                }
                exportCSV("source6_time_evolution.csv", timeSeriesData,
                          {"time", "Ug1", "Um", "FU"});
            }
            break;

        case 6:
            std::cout << "\n=== Modify Parameters ===" << std::endl;
            std::cout << "1. Change body (current: " << bodies[currentBodyIndex].name << ")" << std::endl;
            std::cout << "2. Change distance r (current: " << sim.r << ")" << std::endl;
            std::cout << "3. Change time t (current: " << sim.t << ")" << std::endl;
            std::cout << "Enter choice: ";
            int modChoice;
            std::cin >> modChoice;

            if (modChoice == 1)
            {
                std::cout << "Select body:" << std::endl;
                for (size_t i = 0; i < bodies.size(); ++i)
                {
                    std::cout << i << ". " << bodies[i].name << std::endl;
                }
                std::cout << "Enter index: ";
                std::cin >> currentBodyIndex;
                if (currentBodyIndex < 0 || currentBodyIndex >= (int)bodies.size())
                {
                    currentBodyIndex = 0;
                }
            }
            else if (modChoice == 2)
            {
                std::cout << "Enter new r (m): ";
                std::cin >> sim.r;
            }
            else if (modChoice == 3)
            {
                std::cout << "Enter new t (s): ";
                std::cin >> sim.t;
                sim.tn = sim.t;
            }
            break;

        case 7:
            std::cout << "Exiting simulation harness." << std::endl;
            break;

        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }

    return 0;
}

/*
 * COMPILATION INSTRUCTIONS:
 *
 * g++ -std=c++17 source6_simulation_harness.cpp -o source6_harness
 * ./source6_harness
 *
 * For full integration with source6_wolfram.cpp:
 * g++ -std=c++17 source6_wolfram.cpp source6_simulation_harness.cpp -o source6_harness_full
 *
 * Note: Requires PhysicsTermRegistry implementation to evaluate actual physics terms.
 *       Current version provides structure and placeholder outputs.
 */
