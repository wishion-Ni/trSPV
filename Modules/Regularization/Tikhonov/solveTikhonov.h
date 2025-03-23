#pragma once
// tikhonov_solver.h
#pragma once

#include <Eigen/Dense>
#include <string>

namespace trSPV {

    enum class SolverType {
        SVD,
        QR,
        Cholesky,
        CG,
        LSQR,
        GSVD // reserved for future
    };

    struct TikhonovOptions {
        double lambda = 1e-3; // regularization strength
        Eigen::MatrixXd L;   // regularization matrix (optional, default to identity)
        SolverType solver = SolverType::SVD;
        int maxIterations = 1000; // for iterative solvers
        double tolerance = 1e-6;  // for iterative solvers
    };

    struct TikhonovResult {
        Eigen::VectorXd x;       // solution vector
        double residual = 0.0;   // ||Ax - b||
        double regularization = 0.0; // ||Lx||
        double cost = 0.0;       // total cost: residual^2 + lambda^2 * regularization^2
        int iterations = 0;      // number of iterations (for iterative solvers)
    };

    // Unified interface
    TikhonovResult solveTikhonov(
        const Eigen::MatrixXd& A,
        const Eigen::VectorXd& b,
        const TikhonovOptions& options
    );

    // SVD-based implementation (L = I only)
    TikhonovResult solveTikhonovSVD(
        const Eigen::MatrixXd& A,
        const Eigen::VectorXd& b,
        double lambda
    );

    TikhonovResult solveTikhonovQR(
        const Eigen::MatrixXd& A,
        const Eigen::VectorXd& b,
        double lambda
    );

    TikhonovResult solveTikhonovCholesky(
        const Eigen::MatrixXd& A,
        const Eigen::VectorXd& b,
        double lambda
    );

    TikhonovResult solveTikhonovCG(
        const Eigen::MatrixXd& A,
        const Eigen::VectorXd& b,
        double lambda,
        int maxIterations,
        double tolerance
    );

    TikhonovResult solveTikhonovLSQR(
        const Eigen::MatrixXd& A,
        const Eigen::VectorXd& b,
        double lambda,
        int maxIterations,
        double tolerance
    );

} // namespace trSPV
