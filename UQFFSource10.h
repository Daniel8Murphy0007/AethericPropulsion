#ifndef UQFF_SOURCE10_H
#define UQFF_SOURCE10_H

#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <random>

namespace UQFF
{

    class Source10
    {
    private:
        std::map<std::string, double> scaling_factors;
        std::mt19937 rng;

    public:
        Source10() : rng(std::random_device{}())
        {
            // Initialize default scaling factors
            scaling_factors["default"] = 1.0;
        }

        void loadConfig(const std::string &config_file);

        // Batch compute methods
        std::vector<double> batch_compute_F_U_Bi_i(const std::vector<double> &inputs);

        // Core UQFF computations
        double compute_F_U_Bi_i(double param);
        double compute_g_UQFF(double r, double t);

        // Getter for scaling factors
        double getScalingFactor(const std::string &key) const
        {
            auto it = scaling_factors.find(key);
            return (it != scaling_factors.end()) ? it->second : 1.0;
        }
    };

} // namespace UQFF

#endif // UQFF_SOURCE10_H
