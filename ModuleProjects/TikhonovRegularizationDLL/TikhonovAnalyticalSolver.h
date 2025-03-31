#pragma once

#include <vector>
#include <complex>

class TikhonovAnalyticalSolver {
public:
    enum SolverType { LU, QR, Cholesky, SVD };

    TikhonovAnalyticalSolver(const std::vector<std::complex<double>>& A, int m, int n,
        const std::vector<std::complex<double>>& b);

    void setLambda(double lambda);
    void setRegularizationMatrix(const std::vector<std::complex<double>>& L, int l_rows, int l_cols);
    void useIdentityRegularization();
    void setSolverType(SolverType type);

    std::vector<std::complex<double>> solve();

private:
    int m_, n_;
    std::vector<std::complex<double>> A_, b_;
    std::vector<std::complex<double>> L_;
    int l_rows_ = 0, l_cols_ = 0;
    double lambda_ = 0.0;
    SolverType solverType_ = LU;

    void buildNormalEquation(std::vector<std::complex<double>>& lhs,
        std::vector<std::complex<double>>& rhs);

    std::vector<std::complex<double>> solveLinearSystem(const std::vector<std::complex<double>>& lhs,
        const std::vector<std::complex<double>>& rhs,
        int dim);
};