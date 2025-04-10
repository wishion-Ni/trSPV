#include "TauGridGenerator.h"
#include <cmath>
#include <stdexcept>
#include <algorithm>

std::vector<double> TauGridGenerator::generate(const std::string& type, double tau_min, double tau_max, int num_points) {
    std::vector<double> tau;
    if (num_points <= 1) throw std::invalid_argument("[TauGridGenerator] num_points must be > 1");

    if (type == "log") {
        double log_min = std::log10(tau_min);
        double log_max = std::log10(tau_max);
        double delta = (log_max - log_min) / (num_points - 1);
        for (int i = 0; i < num_points; ++i) {
            tau.push_back(std::pow(10.0, log_min + i * delta));
        }
    }
    else if (type == "linear") {
        double delta = (tau_max - tau_min) / (num_points - 1);
        for (int i = 0; i < num_points; ++i) {
            tau.push_back(tau_min + i * delta);
        }
    }
    else {
        throw std::invalid_argument("[TauGridGenerator] Unsupported tau type: " + type);
    }
    return tau;
}

std::vector<double> TauGridGenerator::generateAuto(
    const std::string& type,
    const std::vector<double>& periods,
    int num_points,
    double factor_low,
    double factor_high
) {
    if (periods.empty()) throw std::invalid_argument("[TauGridGenerator] period array is empty.");
    double T_min = *std::min_element(periods.begin(), periods.end());
    double T_max = *std::max_element(periods.begin(), periods.end());

    double tau_min = T_min * factor_low;
    double tau_max = T_max * factor_high;

    return generate(type, tau_min, tau_max, num_points);
}