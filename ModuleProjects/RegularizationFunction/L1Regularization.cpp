#include "L1Regularization.h"
#include <cmath>

double L1Regularization::evaluate(const std::vector<std::complex<double>>& x) const {
    double sum = 0.0;
    for (const auto& xi : x) {
        sum += std::abs(xi);
    }
    return sum;
}

std::vector<std::complex<double>> L1Regularization::gradient(const std::vector<std::complex<double>>& x) const {
    std::vector<std::complex<double>> grad;
    grad.reserve(x.size());
    for (const auto& xi : x) {
        double norm = std::abs(xi);
        if (norm > 1e-12)
            grad.push_back(xi / norm);
        else
            grad.push_back({ 0.0, 0.0 });
    }
    return grad;
}
