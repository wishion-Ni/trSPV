#include "InitialGuessGenerator.h"
#include <random>
#include <stdexcept>
#include <fstream>
#include <sstream>

std::vector<double> InitialGuessGenerator::generate(const std::string& mode,
    size_t n,
    double noise_level,
    const std::vector<double>& tikhonov_x,
    const std::string& custom_path) {
    std::vector<double> x(n, 0.0);

    if (mode == "zero") {
        return x;
    }
    else if (mode == "noisy") {
        std::default_random_engine gen(std::random_device{}());
        std::normal_distribution<double> dist(0.0, noise_level);
        for (size_t i = 0; i < n; ++i) x[i] = dist(gen);
        return x;
    }
    else if (mode == "tikhonov") {
        if (tikhonov_x.size() != n) throw std::runtime_error("[InitialGuessGenerator] tikhonov_x size mismatch.");
        return tikhonov_x;
    }
    else if (mode == "custom") {
        std::ifstream fin(custom_path);
        if (!fin) throw std::runtime_error("[InitialGuessGenerator] Failed to open " + custom_path);

        std::vector<double> custom;
        std::string line;
        while (std::getline(fin, line)) {
            std::istringstream iss(line);
            double val;
            if (iss >> val) custom.push_back(val);
        }
        if (custom.size() != n) throw std::runtime_error("[InitialGuessGenerator] custom_x0 size mismatch with tau grid");
        return custom;
    }
    else {
        throw std::invalid_argument("[InitialGuessGenerator] Unsupported mode: " + mode);
    }
}