#pragma once

#include <string>
#include <vector>

class TauGridGenerator {
public:
    static std::vector<double> generate(const std::string& type, double tau_min, double tau_max, int num_points);

    static std::vector<double> generateAuto(
        const std::string& type,
        const std::vector<double>& periods,
        int num_points,
        double factor_low = 0.1,
        double factor_high = 10.0
    );

};