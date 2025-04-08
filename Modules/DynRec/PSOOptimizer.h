#pragma once

#include "Optimizer.h"
#include <nlohmann/json.hpp>

class PSOOptimizer : public Optimizer {
public:
    PSOOptimizer() = default;

    explicit PSOOptimizer(const nlohmann::json& config) {
        if (config.contains("swarm_size")) swarm_size = config["swarm_size"].get<int>();
        if (config.contains("max_iter")) max_iter = config["max_iter"].get<int>();
        if (config.contains("inertia")) inertia = config["inertia"].get<double>();
        if (config.contains("c1")) c1 = config["c1"].get<double>();
        if (config.contains("c2")) c2 = config["c2"].get<double>();
        if (config.contains("refine")) local_refine = config["refine"].get<bool>();
    }

    std::vector<double> optimize(const OptimizationProblem& problem) override;

private:
    int swarm_size = 30;
    int max_iter = 100;
    double inertia = 0.7;
    double c1 = 1.5;
    double c2 = 1.5;
    bool local_refine = false;
};