#include "TikhonovParameterSelector.h"
#include <cmath>
#include <limits>
#include <fstream>

void TikhonovParameterSelector::setProblem(const std::vector<std::complex<double>>& A, int m, int n,
    const std::vector<std::complex<double>>& b,
    const std::vector<std::complex<double>>& L,
    int l_rows, int l_cols) {
    A_ = A; b_ = b; L_ = L;
    m_ = m; n_ = n;
    l_rows_ = l_rows; l_cols_ = l_cols;
}

void TikhonovParameterSelector::setCriterion(CriterionType criterion) {
    criterion_ = criterion;
}

void TikhonovParameterSelector::setScanStrategy(ScanStrategy strategy) {
    scan_strategy_ = strategy;
}

void TikhonovParameterSelector::setLcurveCornerMethod(LcurveCornerMethod method) {
    lcurve_method_ = method;
}

void TikhonovParameterSelector::setValidationSet(const std::vector<std::complex<double>>& A_val,
    const std::vector<std::complex<double>>& b_val,
    int m_val) {
    A_val_ = A_val; b_val_ = b_val;
    m_val_ = m_val;
}

void TikhonovParameterSelector::setLambdaList(const std::vector<double>& lambdas) {
    lambda_list_ = lambdas;
}

void TikhonovParameterSelector::setLogSpace(double lambda_min, double lambda_max, int num_points) {
    lambda_list_.clear();
    double log_min = std::log10(lambda_min);
    double log_max = std::log10(lambda_max);
    for (int i = 0; i < num_points; ++i) {
        double log_val = log_min + i * (log_max - log_min) / (num_points - 1);
        lambda_list_.push_back(std::pow(10, log_val));
    }
}

double TikhonovParameterSelector::findBestLambda() {
    if (lambda_list_.empty()) {
        generateLambdaList();
    }
    switch (criterion_) {
    case CriterionType::Lcurve:
        return evaluateLcurve();
    case CriterionType::Validation:
        return evaluateValidationError();
    case CriterionType::GCV:
        return evaluateGCV();
    }
    return -1.0;
}

void TikhonovParameterSelector::generateLambdaList() {
    setLogSpace(1e-6, 1e2, 50);
}

double TikhonovParameterSelector::evaluateGCV() {
    double best_lambda = lambda_list_[0];
    double min_gcv = std::numeric_limits<double>::max();

    for (auto lambda : lambda_list_) {
        TikhonovAnalyticalSolver solver(A_, m_, n_, b_);
        solver.setRegularizationMatrix(L_, l_rows_, l_cols_);
        solver.setLambda(lambda);
        auto x = solver.solve();

        // Compute residual norm
        double res_norm = 0.0;
        for (int i = 0; i < m_; ++i) {
            std::complex<double> tmp = 0.0;
            for (int j = 0; j < n_; ++j) tmp += A_[i * n_ + j] * x[j];
            res_norm += std::norm(tmp - b_[i]);
        }

        // Approximate trace of influence matrix (simplified)
        double trace = static_cast<double>(n_); // fallback, true trace needs SVD
        double gcv_score = res_norm / ((m_ - trace) * (m_ - trace) + 1e-12);

        if (gcv_score < min_gcv) {
            min_gcv = gcv_score;
            best_lambda = lambda;
        }
    }
    return best_lambda;
}

double TikhonovParameterSelector::evaluateValidationError() {
    double best_lambda = lambda_list_[0];
    double min_error = std::numeric_limits<double>::max();

    for (auto lambda : lambda_list_) {
        TikhonovAnalyticalSolver solver(A_, m_, n_, b_);
        solver.setRegularizationMatrix(L_, l_rows_, l_cols_);
        solver.setLambda(lambda);
        auto x = solver.solve();

        double err = 0.0;
        for (int i = 0; i < m_val_; ++i) {
            std::complex<double> tmp = 0.0;
            for (int j = 0; j < n_; ++j) tmp += A_val_[i * n_ + j] * x[j];
            err += std::norm(tmp - b_val_[i]);
        }
        if (err < min_error) {
            min_error = err;
            best_lambda = lambda;
        }
    }
    return best_lambda;
}

double TikhonovParameterSelector::evaluateLcurve() {
    auto curve = getLcurveCurve();
    int best_index = 0;

    if (lcurve_method_ == LcurveCornerMethod::MaxCurvature) {
        double max_curvature = -1.0;
        for (size_t i = 1; i + 1 < curve.size(); ++i) {
            double x1 = curve[i - 1].first, y1 = curve[i - 1].second;
            double x2 = curve[i].first, y2 = curve[i].second;
            double x3 = curve[i + 1].first, y3 = curve[i + 1].second;

            double a = std::hypot(x2 - x1, y2 - y1);
            double b = std::hypot(x3 - x2, y3 - y2);
            double c = std::hypot(x3 - x1, y3 - y1);

            double s = 0.5 * (a + b + c);
            double area = std::sqrt(std::max(s * (s - a) * (s - b) * (s - c), 0.0));
            double curvature = 4 * area / (a * b * c + 1e-12);

            if (curvature > max_curvature) {
                max_curvature = curvature;
                best_index = static_cast<int>(i);
            }
        }
    }
    else if (lcurve_method_ == LcurveCornerMethod::TriangleMethod) {
        const auto& p1 = curve.front();
        const auto& p2 = curve.back();
        double max_dist = -1.0;

        for (size_t i = 1; i + 1 < curve.size(); ++i) {
            const auto& p = curve[i];
            double num = std::abs((p2.second - p1.second) * p.first - (p2.first - p1.first) * p.second + p2.first * p1.second - p2.second * p1.first);
            double den = std::hypot(p2.first - p1.first, p2.second - p1.second);
            double dist = num / (den + 1e-12);
            if (dist > max_dist) {
                max_dist = dist;
                best_index = static_cast<int>(i);
            }
        }
    }

    return lambda_list_[best_index];
}

std::vector<std::pair<double, double>> TikhonovParameterSelector::getLcurveCurve() {
    std::vector<std::pair<double, double>> curve;
    for (auto lambda : lambda_list_) {
        TikhonovAnalyticalSolver solver(A_, m_, n_, b_);
        solver.setRegularizationMatrix(L_, l_rows_, l_cols_);
        solver.setLambda(lambda);
        auto x = solver.solve();

        std::complex<double> rho = 0.0, eta = 0.0;
        for (int i = 0; i < m_; ++i) {
            std::complex<double> tmp = 0.0;
            for (int j = 0; j < n_; ++j) tmp += A_[i * n_ + j] * x[j];
            rho += std::norm(tmp - b_[i]);
        }
        for (int i = 0; i < l_rows_; ++i) {
            std::complex<double> tmp = 0.0;
            for (int j = 0; j < l_cols_; ++j) tmp += L_[i * l_cols_ + j] * x[j];
            eta += std::norm(tmp);
        }
        curve.emplace_back(std::sqrt(rho.real()), std::sqrt(eta.real()));
    }
    return curve;
}

bool TikhonovParameterSelector::exportLcurveToCSV(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) return false;

    file << "lambda,log_residual,log_regularization\n";
    auto curve = getLcurveCurve();
    for (size_t i = 0; i < lambda_list_.size(); ++i) {
        double lambda = lambda_list_[i];
        double log_rho = std::log10(curve[i].first + 1e-12);
        double log_eta = std::log10(curve[i].second + 1e-12);
        file << lambda << "," << log_rho << "," << log_eta << "\n";
    }
    return true;
}
