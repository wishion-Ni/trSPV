#pragma once

#include "Optimizer.h"
#include <nlohmann/json.hpp>

class DEOptimizer : public Optimizer {
public:
    DEOptimizer() = default;

    explicit DEOptimizer(const nlohmann::json& config) {
        if (config.contains("pop_size")) pop_size = config["pop_size"].get<int>();
        if (config.contains("max_iter")) max_iter = config["max_iter"].get<int>();
        if (config.contains("F")) F = config["F"].get<double>();
        if (config.contains("CR")) CR = config["CR"].get<double>();
        if (config.contains("refine")) local_refine = config["refine"].get<bool>();
    }

    std::vector<double> optimize(const OptimizationProblem& problem) override;

private:
    int pop_size = 30;
    int max_iter = 100;
    double F = 0.8;      // 差分权重因子
    double CR = 0.9;     // 交叉概率
    bool local_refine = false;
};
