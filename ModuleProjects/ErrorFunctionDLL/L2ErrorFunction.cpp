#include "L2ErrorFunction.h"
#include <stdexcept>

double L2ErrorFunction::evaluate(const std::vector<double>& prediction,
    const std::vector<double>& target) const {
    if (prediction.size() != target.size()) {
        throw std::invalid_argument("L2ErrorFunction::evaluate - size mismatch.");
    }

    double sum = 0.0;
    for (size_t i = 0; i < prediction.size(); ++i) {
        double diff = prediction[i] - target[i];
        sum += diff * diff;
    }

    return 0.5 * sum;
}

std::vector<double> L2ErrorFunction::gradient(const std::vector<double>& prediction,
    const std::vector<double>& target) const {
    if (prediction.size() != target.size()) {
        throw std::invalid_argument("L2ErrorFunction::gradient - size mismatch.");
    }

    std::vector<double> grad(prediction.size());
    for (size_t i = 0; i < prediction.size(); ++i) {
        grad[i] = prediction[i] - target[i];
    }

    return grad;
}
