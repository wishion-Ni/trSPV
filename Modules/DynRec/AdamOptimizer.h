#pragma once

#include "Optimizer.h"
#include <nlohmann/json.hpp>

class AdamOptimizer : public Optimizer {
public:
    AdamOptimizer() = default;

    explicit AdamOptimizer(const nlohmann::json& config) {
        if (config.contains("learning_rate")) learning_rate = config["learning_rate"].get<double>();
        if (config.contains("beta1")) beta1 = config["beta1"].get<double>();
        if (config.contains("beta2")) beta2 = config["beta2"].get<double>();
        if (config.contains("epsilon")) epsilon = config["epsilon"].get<double>();
        if (config.contains("max_iter")) max_iter = config["max_iter"].get<int>();
        if (config.contains("tolerance")) tolerance = config["tolerance"].get<double>();
    }

    std::vector<double> optimize(const OptimizationProblem& problem) override;

private:
    double learning_rate = 1e-3;
    double beta1 = 0.9;
    double beta2 = 0.999;
    double epsilon = 1e-8;
    int max_iter = 100;
    double tolerance = 1e-6;
};