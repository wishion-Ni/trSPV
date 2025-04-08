#pragma once

#include <vector>
#include <complex>
#include <memory>
#include "ForwardModel.h"
#include "../../Plugins/lib/ComplexLossFunction.h"

struct OptimizationProblem {
    std::shared_ptr<ForwardModel> model;
    std::shared_ptr<ComplexLossFunction> loss;
    void* reg_handle = nullptr;
    double reg_weight = 0.0;
    std::vector<std::complex<double>> target;
    std::vector<double> x0;
};