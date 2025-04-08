#pragma once

#include "Optimizer.h"
#include <nlohmann/json.hpp>

class ConjugateGradientOptimizer : public Optimizer {
public:
    ConjugateGradientOptimizer() = default;
    explicit ConjugateGradientOptimizer(const nlohmann::json& config) {
        if (config.contains("max_iter")) max_iter = config["max_iter"].get<int>();
        if (config.contains("tolerance")) tolerance = config["tolerance"].get<double>();
        if (config.contains("variant")) cg_variant = config["variant"].get<std::string>();
    }

    std::vector<double> optimize(const OptimizationProblem& problem) override;

private:
    int max_iter = 100;
    double tolerance = 1e-6;
    std::string cg_variant = "FR";
};