#include "ComplexLogCoshLoss.h"

double ComplexLogCoshLoss::evaluate(const std::vector<std::complex<double>>& prediction,
    const std::vector<std::complex<double>>& target) const {
    if (prediction.size() != target.size()) throw std::invalid_argument("LogCosh: size mismatch");
    double sum = 0.0;
    for (size_t i = 0; i < prediction.size(); ++i) {
        double mag = std::abs(prediction[i] - target[i]);
        sum += std::log(std::cosh(mag));
    }
    return sum;
}

std::vector<std::complex<double>> ComplexLogCoshLoss::gradient(const std::vector<std::complex<double>>& prediction,
    const std::vector<std::complex<double>>& target) const {
    if (prediction.size() != target.size()) throw std::invalid_argument("LogCosh grad: size mismatch");
    std::vector<std::complex<double>> grad(prediction.size());
    for (size_t i = 0; i < prediction.size(); ++i) {
        std::complex<double> diff = prediction[i] - target[i];
        double mag = std::abs(diff);
        grad[i] = (mag == 0.0) ? std::complex<double>(0.0, 0.0) : std::tanh(mag) * (diff / mag);
    }
    return grad;
}