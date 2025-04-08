#pragma once

#include "Optimizer.h"
#include <nlohmann/json.hpp>

class GradientDescentOptimizer : public Optimizer {
public:
    GradientDescentOptimizer() = default;
    explicit GradientDescentOptimizer(const nlohmann::json& config) {
        if (config.contains("learning_rate")) learning_rate = config["learning_rate"].get<double>();
        if (config.contains("max_iter")) max_iter = config["max_iter"].get<int>();
        if (config.contains("tolerance")) tolerance = config["tolerance"].get<double>();
    }

    std::vector<double> optimize(const OptimizationProblem& problem) override;

private:
    double learning_rate = 1e-2;
    int max_iter = 100;
    double tolerance = 1e-6;
};