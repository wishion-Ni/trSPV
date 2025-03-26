#include "L1ErrorFunction.h"
#include <stdexcept>

double L1ErrorFunction::evaluate(const std::vector<double>& prediction,
    const std::vector<double>& target) const {
    if (prediction.size() != target.size()) {
        throw std::invalid_argument("L1ErrorFunction::evaluate - size mismatch.");
    }
    double sum = 0.0;
    for (size_t i = 0; i < prediction.size(); ++i) {
        sum += std::abs(prediction[i] - target[i]);
    }
    return sum;
}

std::vector<double> L1ErrorFunction::gradient(const std::vector<double>& prediction,
    const std::vector<double>& target) const {
    if (prediction.size() != target.size()) {
        throw std::invalid_argument("L1ErrorFunction::gradient - size mismatch.");
    }
    std::vector<double> grad(prediction.size());
    for (size_t i = 0; i < prediction.size(); ++i) {
        double diff = prediction[i] - target[i];
        grad[i] = (diff > 0) ? 1.0 : (diff < 0) ? -1.0 : 0.0; // ´ÎÌÝ¶È
    }
    return grad;
}
