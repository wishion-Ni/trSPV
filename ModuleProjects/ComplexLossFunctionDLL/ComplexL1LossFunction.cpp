#include "ComplexL1LossFunction.h"
#include <stdexcept>

double ComplexL1LossFunction::evaluate(const std::vector<std::complex<double>>& prediction,
    const std::vector<std::complex<double>>& target) const {
    if (prediction.size() != target.size()) throw std::invalid_argument("L1: size mismatch");
    double sum = 0.0;
    for (size_t i = 0; i < prediction.size(); ++i) sum += std::abs(prediction[i] - target[i]);
    return sum;
}

std::vector<std::complex<double>> ComplexL1LossFunction::gradient(const std::vector<std::complex<double>>& prediction,
    const std::vector<std::complex<double>>& target) const {
    if (prediction.size() != target.size()) throw std::invalid_argument("L1: size mismatch");
    std::vector<std::complex<double>> grad(prediction.size());
    for (size_t i = 0; i < prediction.size(); ++i) {
        std::complex<double> diff = prediction[i] - target[i];
        double mag = std::abs(diff);
        grad[i] = (mag == 0.0) ? std::complex<double>(0.0, 0.0) : diff / mag;
    }
    return grad;
}