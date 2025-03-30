#include "ComplexIoULoss.h"
#include <stdexcept>
#include <algorithm>
#include <cmath>

double ComplexIoULoss::evaluate(const std::vector<std::complex<double>>& prediction,
    const std::vector<std::complex<double>>& target) const {
    if (prediction.size() != target.size()) throw std::invalid_argument("IoU: size mismatch");
    double intersection = 0.0;
    double union_area = 0.0;
    for (size_t i = 0; i < prediction.size(); ++i) {
        double a = std::abs(prediction[i]);
        double b = std::abs(target[i]);
        intersection += std::min(a, b);
        union_area += std::max(a, b);
    }
    return 1.0 - (union_area > 0.0 ? intersection / union_area : 0.0);
}

std::vector<std::complex<double>> ComplexIoULoss::gradient(
    const std::vector<std::complex<double>>& prediction,
    const std::vector<std::complex<double>>& target) const {
    // 通常用于评估，梯度为零
    return std::vector<std::complex<double>>(prediction.size(), { 0.0, 0.0 });
}