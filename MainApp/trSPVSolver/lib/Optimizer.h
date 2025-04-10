#pragma once

#include <vector>
#include "OptimizationProblem.h"
#include "DynRecLogger.h"

class Optimizer {
public:
    virtual ~Optimizer() = default;

    // 在 problem.x0 基础上优化，返回最优 x
    virtual std::vector<double> optimize(const OptimizationProblem& problem) = 0;
    virtual void setLogger(DynRecLogger* logger) {
        logger_ = logger;
    }

protected:
    DynRecLogger* logger_ = nullptr;
};
