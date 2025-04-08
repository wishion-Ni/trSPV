#pragma once

#include <vector>
#include "OptimizationProblem.h"

class Optimizer {
public:
    virtual ~Optimizer() = default;

    // 在 problem.x0 基础上优化，返回最优 x
    virtual std::vector<double> optimize(const OptimizationProblem& problem) = 0;
};
