#include "ComplexL2LossFunction.h"
#include <stdexcept>

double ComplexL2LossFunction::evaluate(const std::vector<std::complex<double>>& prediction,
    const std::vector<std::complex<double>>& target) const {
    if (prediction.size() != target.size()) {
        throw std::invalid_argument("ComplexL2LossFunction::evaluate - size mismatch.");
    }
    double sum = 0.0;
    for (size_t i = 0; i < prediction.size(); ++i) {
        std::complex<double> diff = prediction[i] - target[i];
        sum += std::norm(diff); // |z|^2 = Re² + Im²
    }
    return sum;
}

std::vector<std::complex<double>> ComplexL2LossFunction::gradient(
    const std::vector<std::complex<double>>& prediction,
    const std::vector<std::complex<double>>& target) const {

    if (prediction.size() != target.size()) {
        throw std::invalid_argument("ComplexL2LossFunction::gradient - size mismatch.");
    }
    std::vector<std::complex<double>> grad(prediction.size());
    for (size_t i = 0; i < prediction.size(); ++i) {
        grad[i] = prediction[i] - target[i]; // 复数梯度 = 残差（共轭不用加）
    }
    return grad;
}