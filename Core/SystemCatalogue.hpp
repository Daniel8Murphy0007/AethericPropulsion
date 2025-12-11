#ifndef CORE_SYSTEM_CATALOGUE_HPP
#define CORE_SYSTEM_CATALOGUE_HPP

#include <string>
#include <vector>
#include <map>
#include <memory>

namespace Core
{

    // System type enumeration for UQFF classification
    enum class SystemType
    {
        MAGNETAR,
        SUPERMASSIVE_BLACK_HOLE,
        GALAXY,
        STAR_FORMING_REGION,
        NEBULA,
        PLANETARY_SYSTEM,
        QUASAR,
        STELLAR_CLUSTER,
        UNKNOWN
    };

    // Astrophysical system parameters catalog
    struct SystemParameters
    {
        std::string name;
        SystemType type;

        // Physical properties
        double mass;           // kg
        double radius;         // m
        double distance;       // m (from observer/reference point)
        double magnetic_field; // T (Tesla)
        double redshift;       // z (dimensionless)
        double luminosity;     // W (Watts)
        double temperature;    // K (Kelvin)

        // UQFF-specific parameters
        double system_volume;         // m³
        double dark_matter_mass;      // kg
        double vacuum_energy_density; // J/m³

        // Observational data
        std::string catalog_id;                      // NGC, IC, Messier, etc.
        double observation_epoch;                    // Julian date
        std::map<std::string, double> custom_params; // Extensible parameters

        SystemParameters()
            : name("Unknown"), type(SystemType::UNKNOWN),
              mass(0.0), radius(0.0), distance(0.0),
              magnetic_field(0.0), redshift(0.0), luminosity(0.0),
              temperature(0.0), system_volume(0.0),
              dark_matter_mass(0.0), vacuum_energy_density(0.0),
              observation_epoch(0.0) {}
    };

    // System catalogue class for managing astrophysical systems
    class SystemCatalogue
    {
    private:
        std::map<std::string, SystemParameters> systems;

    public:
        SystemCatalogue() { initializeDefaultSystems(); }

        // Add a system to the catalogue
        void addSystem(const std::string &id, const SystemParameters &params)
        {
            systems[id] = params;
        }

        // Get system parameters by ID
        SystemParameters getSystem(const std::string &id) const
        {
            auto it = systems.find(id);
            if (it != systems.end())
                return it->second;
            return SystemParameters(); // Return default if not found
        }

        // Check if system exists
        bool hasSystem(const std::string &id) const
        {
            return systems.find(id) != systems.end();
        }

        // Get all system IDs
        std::vector<std::string> getSystemIDs() const
        {
            std::vector<std::string> ids;
            for (const auto &pair : systems)
                ids.push_back(pair.first);
            return ids;
        }

        // Get count of systems
        size_t getSystemCount() const { return systems.size(); }

    private:
        // Initialize default astrophysical systems from source10.cpp context
        void initializeDefaultSystems()
        {
            // SGR 1745 Magnetar (Galactic Center)
            SystemParameters sgr1745;
            sgr1745.name = "SGR 1745-2900";
            sgr1745.type = SystemType::MAGNETAR;
            sgr1745.mass = 2.984e30;       // ~1.5 solar masses
            sgr1745.radius = 12000.0;      // ~12 km
            sgr1745.distance = 2.55e20;    // ~26,000 ly
            sgr1745.magnetic_field = 1e10; // 10^10 T
            sgr1745.luminosity = 3.8e32;   // W
            sgr1745.catalog_id = "SGR 1745-2900";
            systems["SGR1745"] = sgr1745;

            // Sagittarius A* (Supermassive Black Hole)
            SystemParameters sgra;
            sgra.name = "Sagittarius A*";
            sgra.type = SystemType::SUPERMASSIVE_BLACK_HOLE;
            sgra.mass = 8.155e36;          // ~4.1 million solar masses
            sgra.radius = 1.2e10;          // Schwarzschild radius ~12 million km
            sgra.distance = 2.55e20;       // ~26,000 ly
            sgra.dark_matter_mass = 1e37;  // Estimated DM halo
            sgra.system_volume = 3.552e45; // m³
            sgra.catalog_id = "Sgr A*";
            systems["SGRA_STAR"] = sgra;

            // M82 (Starburst Galaxy)
            SystemParameters m82;
            m82.name = "M82";
            m82.type = SystemType::GALAXY;
            m82.mass = 5e40;       // kg
            m82.radius = 3.7e20;   // ~12,000 ly diameter
            m82.distance = 1.1e23; // ~12 million ly
            m82.luminosity = 5e37; // W (starburst)
            m82.catalog_id = "M82 / NGC 3034";
            systems["M82"] = m82;

            // Default template for new systems
            SystemParameters template_system;
            template_system.name = "Template System";
            template_system.type = SystemType::UNKNOWN;
            systems["TEMPLATE"] = template_system;
        }
    };

} // namespace Core

#endif // CORE_SYSTEM_CATALOGUE_HPP
