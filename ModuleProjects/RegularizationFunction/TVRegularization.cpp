#include "TVRegularization.h"
#include <cmath>

double TVRegularization::evaluate(const std::vector<std::complex<double>>& x) const {
    double sum = 0.0;
    for (size_t i = 1; i < x.size(); ++i) {
        sum += std::abs(x[i] - x[i - 1]);
    }
    return sum;
}

std::vector<std::complex<double>> TVRegularization::gradient(const std::vector<std::complex<double>>& x) const {
    std::vector<std::complex<double>> grad(x.size(), { 0.0, 0.0 });
    for (size_t i = 1; i < x.size(); ++i) {
        auto diff = x[i] - x[i - 1];
        auto norm = std::abs(diff);
        if (norm > 1e-12) {
            grad[i] += diff / norm;
            grad[i - 1] -= diff / norm;
        }
    }
    return grad;
}
