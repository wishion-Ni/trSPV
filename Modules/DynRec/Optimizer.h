#pragma once

#include <vector>
#include "OptimizationProblem.h"

class Optimizer {
public:
    virtual ~Optimizer() = default;

    // �� problem.x0 �������Ż����������� x
    virtual std::vector<double> optimize(const OptimizationProblem& problem) = 0;
};
