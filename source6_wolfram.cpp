// Wolfram-Enhanced Physics Terms from source6.cpp - MAIN INFRASTRUCTURE
// 4-File Modularization: Main file with structures, base class, and registry
// Generated: November 29, 2025
// Total Classes: 29 (14 graphics + 15 UQFF physics) across 4 files
//
// FILE STRUCTURE:
// 1. source6_wolfram.cpp (THIS FILE): CelestialBody structure, PhysicsTerm base class,
//    PhysicsTermRegistry, registration function
// 2. source6_wolfram_graphics.cpp: 14 graphics infrastructure classes
//    (OpenGLRender, VulkanRender, MeshLoaderOBJ, ProceduralLandscape, MeshExtrude,
//     MeshBoolean, TextureLoader, ShaderCompile, CameraViewMatrix, BoneAnimation,
//     LaTeXRender, MultiViewport, SimulationEntityUpdate, ToolPathExecution)
// 3. source6_wolfram_physics.cpp: 15 UQFF physics classes
//    (7 helpers: StepFunction, ReactorEnergy, MagneticMomentTime, GradientMassRadius,
//                MagneticJetField, OmegaSpinModulation, MagneticJetMoment
//     8 core: UniversalGravity1-4, UniversalBuoyancy, UniversalMagnetism,
//             SpacetimeMetric, FullUnifiedField)
// 4. source6_simulation_harness.cpp: Interactive testing (already exists separately)

#include <cmath>
#include <string>
#include <map>
#include <memory>
#include <vector>
#include <stdexcept>
#include <iostream>

// ============================================================================
// Physics Constants (for UQFF calculations)
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
// PhysicsTerm Base Class (abstract interface for all physics calculations)
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
// PhysicsTermRegistry Class
// ============================================================================
class PhysicsTermRegistry
{
private:
    std::map<std::string, std::unique_ptr<PhysicsTerm>> terms;
    std::map<std::string, std::string> categories;

public:
    void registerPhysicsTerm(const std::string &name, std::unique_ptr<PhysicsTerm> term, const std::string &category)
    {
        terms[name] = std::move(term);
        categories[name] = category;
    }

    PhysicsTerm *getPhysicsTerm(const std::string &name)
    {
        auto it = terms.find(name);
        if (it != terms.end())
        {
            return it->second.get();
        }
        return nullptr;
    }

    std::string getCategory(const std::string &name) const
    {
        auto it = categories.find(name);
        if (it != categories.end())
        {
            return it->second;
        }
        return "";
    }

    std::vector<std::string> getAllTermNames() const
    {
        std::vector<std::string> names;
        for (const auto &pair : terms)
        {
            names.push_back(pair.first);
        }
        return names;
    }

    std::vector<std::string> getTermsByCategory(const std::string &category) const
    {
        std::vector<std::string> names;
        for (const auto &pair : categories)
        {
            if (pair.second == category)
            {
                names.push_back(pair.first);
            }
        }
        return names;
    }

    size_t getTermCount() const
    {
        return terms.size();
    }

    void printRegistry() const
    {
        std::cout << "\n=== Source6 Physics Term Registry ===\n";
        std::cout << "Total Terms: " << terms.size() << "\n\n";

        // Group by category
        std::map<std::string, std::vector<std::string>> grouped;
        for (const auto &pair : categories)
        {
            grouped[pair.second].push_back(pair.first);
        }

        for (const auto &cat_pair : grouped)
        {
            std::cout << cat_pair.first << " (" << cat_pair.second.size() << " terms):\n";
            for (const auto &name : cat_pair.second)
            {
                auto it = terms.find(name);
                if (it != terms.end())
                {
                    std::cout << "  - " << name << ": " << it->second->getDescription() << "\n";
                }
            }
            std::cout << "\n";
        }
    }
};

// ============================================================================
// Forward Declarations (classes defined in separate files)
// ============================================================================

// Graphics infrastructure classes (source6_wolfram_graphics.cpp)
class OpenGLRenderTerm;
class VulkanRenderTerm;
class MeshLoaderOBJTerm;
class ProceduralLandscapeTerm;
class MeshExtrudeTerm;
class MeshBooleanTerm;
class TextureLoaderTerm;
class ShaderCompileTerm;
class CameraViewMatrixTerm;
class BoneAnimationTerm;
class LaTeXRenderTerm;
class MultiViewportTerm;
class SimulationEntityUpdateTerm;
class ToolPathExecutionTerm;

// UQFF physics classes (source6_wolfram_physics.cpp)
class StepFunctionSource6Term;
class ReactorEnergySource6Term;
class MagneticMomentTimeSource6Term;
class GradientMassRadiusSource6Term;
class MagneticJetFieldSource6Term;
class OmegaSpinModulationSource6Term;
class MagneticJetMomentSource6Term;
class UniversalGravity1Source6Term;
class UniversalGravity2Source6Term;
class UniversalGravity3Source6Term;
class UniversalGravity4Source6Term;
class UniversalBuoyancySource6Term;
class UniversalMagnetismSource6Term;
class SpacetimeMetricSource6Term;
class FullUnifiedFieldSource6Term;

// ============================================================================
// Registration Function (registers all 29 physics terms)
// ============================================================================
#include "source6_register.cpp"

// ============================================================================
// Main Function (infrastructure testing)
// ============================================================================
int main()
{
    PhysicsTermRegistry registry;
    registerSource6PhysicsTerms(registry);

    std::cout << "Source6 Wolfram Infrastructure Test\n";
    std::cout << "====================================\n\n";

    registry.printRegistry();

    std::cout << "\n=== Summary ===\n";
    std::cout << "Total registered terms: " << registry.getTermCount() << "\n";
    std::cout << "Graphics infrastructure: " << registry.getTermsByCategory("graphics").size() << "\n";
    std::cout << "UQFF helper terms: " << registry.getTermsByCategory("helper").size() << "\n";
    std::cout << "UQFF gravity terms: " << registry.getTermsByCategory("gravity_wolfram").size() << "\n";
    std::cout << "Unified field terms: " << registry.getTermsByCategory("unified_field").size() << "\n";

    return 0;
}
