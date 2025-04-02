#include "HuberRegularization.h"
#include <cmath>

double HuberRegularization::evaluate(const std::vector<std::complex<double>>& x) const {
    double sum = 0.0;
    for (const auto& xi : x) {
        double norm = std::abs(xi);
        if (norm <= delta_)
            sum += 0.5 * norm * norm;
        else
            sum += delta_ * (norm - 0.5 * delta_);
    }
    return sum;
}

std::vector<std::complex<double>> HuberRegularization::gradient(const std::vector<std::complex<double>>& x) const {
    std::vector<std::complex<double>> grad;
    grad.reserve(x.size());
    for (const auto& xi : x) {
        double norm = std::abs(xi);
        if (norm <= delta_ && norm > 1e-12)
            grad.push_back(xi);
        else if (norm > delta_)
            grad.push_back(delta_ * xi / norm);
        else
            grad.push_back({ 0.0, 0.0 });
    }
    return grad;
}