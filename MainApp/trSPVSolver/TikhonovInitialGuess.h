#pragma once

#include <vector>
#include <complex>
#include <nlohmann/json.hpp>
#include "TikhonovSolverWrapper.h"

std::vector<double> generateTikhonovInitialGuess(
    const std::vector<std::complex<double>>& A_flat,
    const std::vector<std::complex<double>>& b,
    int m, int n,
    const nlohmann::json& config,
    double& lambda_out);