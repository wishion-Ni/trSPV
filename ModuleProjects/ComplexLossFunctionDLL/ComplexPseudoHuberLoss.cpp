#include "ComplexPseudoHuberLoss.h"

ComplexPseudoHuberLoss::ComplexPseudoHuberLoss(double delta_) : delta(delta_) {}

double ComplexPseudoHuberLoss::evaluate(const std::vector<std::complex<double>>& prediction,
    const std::vector<std::complex<double>>& target) const {
    if (prediction.size() != target.size()) throw std::invalid_argument("Huber: size mismatch");
    double sum = 0.0;
    for (size_t i = 0; i < prediction.size(); ++i) {
        double r = std::abs(prediction[i] - target[i]) / delta;
        sum += delta * delta * (std::sqrt(1 + r * r) - 1);
    }
    return sum;
}

std::vector<std::complex<double>> ComplexPseudoHuberLoss::gradient(const std::vector<std::complex<double>>& prediction,
    const std::vector<std::complex<double>>& target) const {
    if (prediction.size() != target.size()) throw std::invalid_argument("Huber grad: size mismatch");
    std::vector<std::complex<double>> grad(prediction.size());
    for (size_t i = 0; i < prediction.size(); ++i) {
        std::complex<double> diff = prediction[i] - target[i];
        double mag = std::abs(diff);
        double scale = 1.0 / std::sqrt(1.0 + (mag / delta) * (mag / delta));
        grad[i] = (mag == 0.0) ? std::complex<double>(0.0, 0.0) : scale * diff;
    }
    return grad;
}