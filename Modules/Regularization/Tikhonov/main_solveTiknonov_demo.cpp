// main_tikhonov_demo.cpp
#include "solveTikhonov.h"
#include <Eigen/Dense>
#include <iostream>
#include <vector>
#include <string>

using namespace trSPV;

void runSolverDemo(const Eigen::MatrixXd& A, const Eigen::VectorXd& b, SolverType solver, const std::string& name) {
    TikhonovOptions opts;
    opts.lambda = 0.1;
    opts.solver = solver;
    opts.maxIterations = 500;
    opts.tolerance = 1e-8;

    std::cout << "\n--- " << name << " ---" << std::endl;
    try {
        TikhonovResult result = solveTikhonov(A, b, opts);
        std::cout << "Residual:      " << result.residual << std::endl;
        std::cout << "Regularization:" << result.regularization << std::endl;
        std::cout << "Cost:          " << result.cost << std::endl;
        if (result.iterations > 0)
            std::cout << "Iterations:    " << result.iterations << std::endl;
    }
    catch (const std::exception& ex) {
        std::cerr << "Solver failed: " << ex.what() << std::endl;
    }
}

int main() {
    constexpr int m = 50; // measurements
    constexpr int n = 30; // unknowns

    // Construct test system: A*x_true = b + noise
    Eigen::MatrixXd A = Eigen::MatrixXd::Random(m, n);
    Eigen::VectorXd x_true = Eigen::VectorXd::LinSpaced(n, 1.0, 2.0);
    Eigen::VectorXd noise = 0.01 * Eigen::VectorXd::Random(m);
    Eigen::VectorXd b = A * x_true + noise;

    runSolverDemo(A, b, SolverType::SVD, "SVD");
    runSolverDemo(A, b, SolverType::QR, "QR");
    runSolverDemo(A, b, SolverType::Cholesky, "Cholesky");
    runSolverDemo(A, b, SolverType::CG, "Conjugate Gradient");
    runSolverDemo(A, b, SolverType::LSQR, "LSQR");

    return 0;
}
