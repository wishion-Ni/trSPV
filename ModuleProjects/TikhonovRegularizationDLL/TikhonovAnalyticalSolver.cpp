#include "TikhonovAnalyticalSolver.h"
#include <Eigen/Dense>

using namespace Eigen;

TikhonovAnalyticalSolver::TikhonovAnalyticalSolver(const std::vector<std::complex<double>>& A, int m, int n,
    const std::vector<std::complex<double>>& b)
    : A_(A), m_(m), n_(n), b_(b) {}

void TikhonovAnalyticalSolver::setLambda(double lambda) {
    lambda_ = lambda;
}

void TikhonovAnalyticalSolver::setRegularizationMatrix(const std::vector<std::complex<double>>& L, int l_rows, int l_cols) {
    L_ = L;
    l_rows_ = l_rows;
    l_cols_ = l_cols;
}

void TikhonovAnalyticalSolver::useIdentityRegularization() {
    L_.clear();
    l_rows_ = l_cols_ = 0;
}

void TikhonovAnalyticalSolver::setSolverType(SolverType type) {
    solverType_ = type;
}

std::vector<std::complex<double>> TikhonovAnalyticalSolver::solve() {
    std::vector<std::complex<double>> lhs, rhs;
    buildNormalEquation(lhs, rhs);
    return solveLinearSystem(lhs, rhs, n_);
}

void TikhonovAnalyticalSolver::buildNormalEquation(std::vector<std::complex<double>>& lhs,
    std::vector<std::complex<double>>& rhs) {
    Map<const MatrixXcd> Amap(A_.data(), m_, n_);
    Map<const VectorXcd> bmap(b_.data(), m_);

    MatrixXcd AtA = Amap.adjoint() * Amap;
    VectorXcd Atb = Amap.adjoint() * bmap;

    if (!L_.empty()) {
        Map<const MatrixXcd> Lmap(L_.data(), l_rows_, l_cols_);
        AtA += lambda_ * lambda_ * (Lmap.adjoint() * Lmap);
    }
    else {
        AtA += lambda_ * lambda_ * MatrixXcd::Identity(n_, n_);
    }

    lhs.assign(AtA.data(), AtA.data() + n_ * n_);
    rhs.assign(Atb.data(), Atb.data() + n_);
}

std::vector<std::complex<double>> TikhonovAnalyticalSolver::solveLinearSystem(
    const std::vector<std::complex<double>>& lhs,
    const std::vector<std::complex<double>>& rhs,
    int dim) {

    Map<const MatrixXcd> A(lhs.data(), dim, dim);
    Map<const VectorXcd> b(rhs.data(), dim);

    VectorXcd x;
    switch (solverType_) {
    case LU:
        x = A.lu().solve(b);
        break;
    case QR:
        x = A.colPivHouseholderQr().solve(b);
        break;
    case Cholesky:
        x = A.selfadjointView<Lower>().llt().solve(b);
        break;
    case SVD:
        x = A.jacobiSvd(ComputeThinU | ComputeThinV).solve(b);
        break;
    }

    return std::vector<std::complex<double>>(x.data(), x.data() + dim);
}
