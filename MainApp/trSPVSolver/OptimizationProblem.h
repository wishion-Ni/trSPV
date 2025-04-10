#pragma once

#include <vector>
#include <complex>
#include <string>

struct OptimizationProblem {
    std::vector<std::complex<double>> A_flat;
    std::vector<std::complex<double>> target;
    std::vector<double> x0;

    int m = 0;
    int n = 0;

    std::string loss_json;
    std::string reg_json;
    std::string optimizer_json;

    double reg_weight = 0.0;
};
