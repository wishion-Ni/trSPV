#include "ModulusSquaredDiffLoss.h"

double ModulusSquaredDiffLoss::evaluate(const std::vector<std::complex<double>>& prediction,
    const std::vector<std::complex<double>>& target) const {
    if (prediction.size() != target.size()) throw std::invalid_argument("ModSquaredDiff: size mismatch");
    double sum = 0.0;
    for (size_t i = 0; i < prediction.size(); ++i) {
        double a = std::norm(prediction[i]);
        double b = std::norm(target[i]);
        sum += (a - b) * (a - b);
    }
    return sum;
}

std::vector<std::complex<double>> ModulusSquaredDiffLoss::gradient(const std::vector<std::complex<double>>& prediction,
    const std::vector<std::complex<double>>& target) const {
    std::vector<std::complex<double>> grad(prediction.size());
    for (size_t i = 0; i < prediction.size(); ++i) {
        double diff = std::norm(prediction[i]) - std::norm(target[i]);
        grad[i] = 4.0 * diff * prediction[i];
    }
    return grad;
}
