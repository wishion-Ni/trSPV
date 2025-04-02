#include "L2Regularization.h"

double L2Regularization::evaluate(const std::vector<std::complex<double>>& x) const {
    double sum = 0.0;
    for (const auto& xi : x) {
        sum += std::norm(xi);
    }
    return sum;
}

std::vector<std::complex<double>> L2Regularization::gradient(const std::vector<std::complex<double>>& x) const {
    std::vector<std::complex<double>> grad;
    grad.reserve(x.size());
    for (const auto& xi : x) {
        grad.push_back(2.0 * xi);
    }
    return grad;
}