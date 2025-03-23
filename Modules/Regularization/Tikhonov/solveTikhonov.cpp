// tikhonov_solver.cpp
#include "solveTikhonov.h"
#include <Eigen/SVD>
#include <Eigen/QR>
#include <Eigen/Cholesky>
#include <Eigen/IterativeLinearSolvers>
#include <iostream>
#include <stdexcept>

namespace trSPV {

    TikhonovResult solveTikhonovSVD(const Eigen::MatrixXd& A, const Eigen::VectorXd& b, double lambda) {
        TikhonovResult result;
        Eigen::JacobiSVD<Eigen::MatrixXd> svd(A, Eigen::ComputeThinU | Eigen::ComputeThinV);
        const auto& U = svd.matrixU();
        const auto& V = svd.matrixV();
        const auto& S = svd.singularValues();

        Eigen::VectorXd x = Eigen::VectorXd::Zero(V.cols());
        for (int i = 0; i < S.size(); ++i) {
            double sigma = S(i);
            double filter = sigma / (sigma * sigma + lambda * lambda);
            x += filter * U.col(i).dot(b) * V.col(i);
        }

        result.x = x;
        result.residual = (A * x - b).norm();
        result.regularization = x.norm();
        result.cost = result.residual * result.residual + lambda * lambda * result.regularization * result.regularization;
        return result;
    }

    TikhonovResult solveTikhonovQR(const Eigen::MatrixXd& A, const Eigen::VectorXd& b, double lambda) {
        TikhonovResult result;
        Eigen::MatrixXd AtA = A.transpose() * A + lambda * lambda * Eigen::MatrixXd::Identity(A.cols(), A.cols());
        Eigen::VectorXd Atb = A.transpose() * b;
        result.x = AtA.colPivHouseholderQr().solve(Atb);
        result.residual = (A * result.x - b).norm();
        result.regularization = result.x.norm();
        result.cost = result.residual * result.residual + lambda * lambda * result.regularization * result.regularization;
        return result;
    }

    TikhonovResult solveTikhonovCholesky(const Eigen::MatrixXd& A, const Eigen::VectorXd& b, double lambda) {
        TikhonovResult result;
        Eigen::MatrixXd AtA = A.transpose() * A + lambda * lambda * Eigen::MatrixXd::Identity(A.cols(), A.cols());
        Eigen::VectorXd Atb = A.transpose() * b;
        result.x = AtA.ldlt().solve(Atb);
        result.residual = (A * result.x - b).norm();
        result.regularization = result.x.norm();
        result.cost = result.residual * result.residual + lambda * lambda * result.regularization * result.regularization;
        return result;
    }

    TikhonovResult solveTikhonovCG(const Eigen::MatrixXd& A, const Eigen::VectorXd& b, double lambda, int maxIterations, double tolerance) {
        TikhonovResult result;
        Eigen::MatrixXd AtA = A.transpose() * A + lambda * lambda * Eigen::MatrixXd::Identity(A.cols(), A.cols());
        Eigen::VectorXd Atb = A.transpose() * b;

        Eigen::ConjugateGradient<Eigen::MatrixXd, Eigen::Lower | Eigen::Upper> cg;
        cg.setMaxIterations(maxIterations);
        cg.setTolerance(tolerance);
        cg.compute(AtA);
        result.x = cg.solve(Atb);

        result.iterations = cg.iterations();
        result.residual = (A * result.x - b).norm();
        result.regularization = result.x.norm();
        result.cost = result.residual * result.residual + lambda * lambda * result.regularization * result.regularization;
        return result;
    }

    TikhonovResult solveTikhonovLSQR(const Eigen::MatrixXd& A, const Eigen::VectorXd& b, double lambda, int maxIterations, double tolerance) {
        TikhonovResult result;
        Eigen::MatrixXd Augmented;
        Augmented.resize(A.rows() + A.cols(), A.cols());
        Augmented.topRows(A.rows()) = A;
        Augmented.bottomRows(A.cols()) = lambda * Eigen::MatrixXd::Identity(A.cols(), A.cols());

        Eigen::VectorXd rhs(b.size() + A.cols());
        rhs << b, Eigen::VectorXd::Zero(A.cols());

        Eigen::LeastSquaresConjugateGradient<Eigen::MatrixXd> lsqr;
        lsqr.setMaxIterations(maxIterations);
        lsqr.setTolerance(tolerance);
        lsqr.compute(Augmented);
        result.x = lsqr.solve(rhs);

        result.iterations = lsqr.iterations();
        result.residual = (A * result.x - b).norm();
        result.regularization = result.x.norm();
        result.cost = result.residual * result.residual + lambda * lambda * result.regularization * result.regularization;
        return result;
    }

    TikhonovResult solveTikhonov(const Eigen::MatrixXd& A, const Eigen::VectorXd& b, const TikhonovOptions& options) {
        switch (options.solver) {
        case SolverType::SVD:
            if (options.L.size() == 0) return solveTikhonovSVD(A, b, options.lambda);
            else throw std::runtime_error("SVD solver currently only supports L = I.");
        case SolverType::QR:
            return solveTikhonovQR(A, b, options.lambda);
        case SolverType::Cholesky:
            return solveTikhonovCholesky(A, b, options.lambda);
        case SolverType::CG:
            return solveTikhonovCG(A, b, options.lambda, options.maxIterations, options.tolerance);
        case SolverType::LSQR:
            return solveTikhonovLSQR(A, b, options.lambda, options.maxIterations, options.tolerance);
        default:
            throw std::runtime_error("Solver not implemented.");
        }
    }

} // namespace trSPV