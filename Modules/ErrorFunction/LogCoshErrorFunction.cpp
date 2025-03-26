#include "LogCoshErrorFunction.h"
#include <stdexcept>

double LogCoshErrorFunction::evaluate(const std::vector<double>& prediction,
    const std::vector<double>& target) const {
    if (prediction.size() != target.size()) {
        throw std::invalid_argument("LogCoshErrorFunction::evaluate - size mismatch.");
    }
    double sum = 0.0;
    for (size_t i = 0; i < prediction.size(); ++i) {
        double diff = prediction[i] - target[i];
        sum += std::log(std::cosh(diff));
    }
    return sum;
}

std::vector<double> LogCoshErrorFunction::gradient(const std::vector<double>& prediction,
    const std::vector<double>& target) const {
    if (prediction.size() != target.size()) {
        throw std::invalid_argument("LogCoshErrorFunction::gradient - size mismatch.");
    }
    std::vector<double> grad(prediction.size());
    for (size_t i = 0; i < prediction.size(); ++i) {
        double diff = prediction[i] - target[i];
        grad[i] = std::tanh(diff);
    }
    return grad;
}