#pragma once

#include <string>
#include <vector>

class InitialGuessGenerator {
public:
    static std::vector<double> generate(const std::string& mode,
        size_t n,
        double noise_level = 1e-4,
        const std::vector<double>& tikhonov_x = {},
        const std::string& custom_path = "custom_x0.txt");
};