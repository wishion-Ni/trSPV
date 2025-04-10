#include "TikhonovInitialGuess.h"
#include <stdexcept>
#include <iostream>

std::vector<double> generateTikhonovInitialGuess(
    const std::vector<std::complex<double>>& A_flat,
    const std::vector<std::complex<double>>& b,
    int m, int n,
    const nlohmann::json& config,
    double& lambda_out) {

    TikhonovSolverWrapper solver;
    if (!solver.load("plugins/TikhonovRegularizationDLL.dll")) {
        throw std::runtime_error("[TikhonovInitialGuess] Failed to load Tikhonov DLL");
    }

    if (!solver.setProblem(A_flat, b, m, n)) {
        throw std::runtime_error("[TikhonovInitialGuess] Failed to set Tikhonov problem");
    }

    if (!solver.configureFromJSON(config)) {
        throw std::runtime_error("[TikhonovInitialGuess] Configuration failed");
    }

    if (!solver.findBestLambda(lambda_out)) {
        throw std::runtime_error("[TikhonovInitialGuess] Failed to find best lambda");
    }

    auto x0 = solver.getSolution(lambda_out);
    return x0;
}
