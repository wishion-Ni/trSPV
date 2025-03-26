#include "HuberErrorFunction.h"
#include <stdexcept>

double HuberErrorFunction::evaluate(const std::vector<double>& prediction,
    const std::vector<double>& target) const {
    if (prediction.size() != target.size()) {
        throw std::invalid_argument("HuberErrorFunction::evaluate - size mismatch.");
    }
    double sum = 0.0;
    for (size_t i = 0; i < prediction.size(); ++i) {
        double diff = prediction[i] - target[i];
        double absDiff = std::abs(diff);
        if (absDiff <= delta_) {
            sum += 0.5 * diff * diff;
        }
        else {
            sum += delta_ * (absDiff - 0.5 * delta_);
        }
    }
    return sum;
}

std::vector<double> HuberErrorFunction::gradient(const std::vector<double>& prediction,
    const std::vector<double>& target) const {
    if (prediction.size() != target.size()) {
        throw std::invalid_argument("HuberErrorFunction::gradient - size mismatch.");
    }
    std::vector<double> grad(prediction.size());
    for (size_t i = 0; i < prediction.size(); ++i) {
        double diff = prediction[i] - target[i];
        if (std::abs(diff) <= delta_) {
            grad[i] = diff;
        }
        else {
            grad[i] = (diff > 0) ? delta_ : -delta_;
        }
    }
    return grad;
}
