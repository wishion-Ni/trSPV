#include "LaplacianRegularization.h"

double LaplacianRegularization::evaluate(const std::vector<std::complex<double>>& x) const {
    double sum = 0.0;
    for (size_t i = 1; i + 1 < x.size(); ++i) {
        auto lap = x[i - 1] - 2.0 * x[i] + x[i + 1];
        sum += std::norm(lap);
    }
    return sum;
}

std::vector<std::complex<double>> LaplacianRegularization::gradient(const std::vector<std::complex<double>>& x) const {
    std::vector<std::complex<double>> grad(x.size(), { 0.0, 0.0 });
    for (size_t i = 1; i + 1 < x.size(); ++i) {
        auto lap = x[i - 1] - 2.0 * x[i] + x[i + 1];
        grad[i - 1] += 2.0 * lap;
        grad[i] -= 4.0 * lap;
        grad[i + 1] += 2.0 * lap;
    }
    return grad;
}
