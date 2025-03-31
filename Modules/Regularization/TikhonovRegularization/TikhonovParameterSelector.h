#pragma once

#include <vector>
#include <complex>
#include <string>
#include "TikhonovAnalyticalSolver.h"

class TikhonovParameterSelector {
public:
    enum class CriterionType { Lcurve, Validation, GCV };
    enum class ScanStrategy { LogSpace, DiscreteList };
    enum class LcurveCornerMethod { MaxCurvature, TriangleMethod };

    void setProblem(const std::vector<std::complex<double>>& A, int m, int n,
        const std::vector<std::complex<double>>& b,
        const std::vector<std::complex<double>>& L,
        int l_rows, int l_cols);

    void setCriterion(CriterionType criterion);
    void setScanStrategy(ScanStrategy strategy);
    void setLcurveCornerMethod(LcurveCornerMethod method);

    void setValidationSet(const std::vector<std::complex<double>>& A_val,
        const std::vector<std::complex<double>>& b_val,
        int m_val);

    void setLambdaList(const std::vector<double>& lambdas);
    void setLogSpace(double lambda_min, double lambda_max, int num_points);

    double findBestLambda();
    std::vector<std::pair<double, double>> getLcurveCurve();
    bool exportLcurveToCSV(const std::string& filename);

private:
    // problem data
    std::vector<std::complex<double>> A_, b_, L_;
    int m_ = 0, n_ = 0, l_rows_ = 0, l_cols_ = 0;

    std::vector<std::complex<double>> A_val_, b_val_;
    int m_val_ = 0;

    std::vector<double> lambda_list_;
    CriterionType criterion_ = CriterionType::Lcurve;
    ScanStrategy scan_strategy_ = ScanStrategy::LogSpace;
    LcurveCornerMethod lcurve_method_ = LcurveCornerMethod::MaxCurvature;

    void generateLambdaList();
    double evaluateLcurve();
    double evaluateValidationError();
    double evaluateGCV();
};