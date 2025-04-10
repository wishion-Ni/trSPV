#include "TikhonovRegularizationDLL.h"
#include "TikhonovParameterSelector.h"
#include <cstring>

extern "C" {

    void* CreateSelector() {
        return new TikhonovParameterSelector();
    }

    void DestroySelector(void* handle) {
        delete static_cast<TikhonovParameterSelector*>(handle);
    }

    void SetProblem(void* handle, const std::complex<double>* A, int m, int n,
        const std::complex<double>* b,
        const std::complex<double>* L, int l_rows, int l_cols) {
        auto* selector = static_cast<TikhonovParameterSelector*>(handle);
        std::vector<std::complex<double>> A_vec(A, A + m * n);
        std::vector<std::complex<double>> b_vec(b, b + m);
        std::vector<std::complex<double>> L_vec(L, L + l_rows * l_cols);
        selector->setProblem(A_vec, m, n, b_vec, L_vec, l_rows, l_cols);
    }

    void SetLambdaLogSpace(void* handle, double lambda_min, double lambda_max, int num_points) {
        auto* selector = static_cast<TikhonovParameterSelector*>(handle);
        selector->setLogSpace(lambda_min, lambda_max, num_points);
    }

    void SetCriterion(void* handle, int criterion) {
        auto* selector = static_cast<TikhonovParameterSelector*>(handle);
        selector->setCriterion(static_cast<TikhonovParameterSelector::CriterionType>(criterion));
    }

    void SetLcurveCornerMethod(void* handle, int method) {
        auto* selector = static_cast<TikhonovParameterSelector*>(handle);
        selector->setLcurveCornerMethod(static_cast<TikhonovParameterSelector::LcurveCornerMethod>(method));
    }

    void SetValidationSet(void* handle, const std::complex<double>* A_val,
        const std::complex<double>* b_val, int m_val) {
        auto* selector = static_cast<TikhonovParameterSelector*>(handle);
        // A_val assumed to be m_val x n (same n)
        // For now assume it's same n as in original problem
        int n = selector->getN();
        std::vector<std::complex<double>> Avec(A_val, A_val + m_val * n);
        std::vector<std::complex<double>> bvec(b_val, b_val + m_val);
        selector->setValidationSet(Avec, bvec, m_val);
    }

    double FindBestLambda(void* handle) {
        auto* selector = static_cast<TikhonovParameterSelector*>(handle);
        return selector->findBestLambda();
    }

    bool ExportLcurveCSV(void* handle, const char* filename) {
        auto* selector = static_cast<TikhonovParameterSelector*>(handle);
        return selector->exportLcurveToCSV(std::string(filename));
    }

    bool GetSolution(const double lambda, void* handle, double* x_out) {
        auto* selector = static_cast<TikhonovParameterSelector*>(handle);
        if (!selector || !x_out) return false;

        TikhonovAnalyticalSolver solver(
            selector->getA(), selector->getM(), selector->getN(), selector->getB());
        solver.setRegularizationMatrix(selector->getL(), selector->getLrows(), selector->getLcols());
        solver.setLambda(lambda);

        auto x_cplx = solver.solve();

        for (int i = 0; i < selector->getN(); ++i) {
            x_out[i] = std::real(x_cplx[i]);
        }

        return true;
    }


} // extern "C"
