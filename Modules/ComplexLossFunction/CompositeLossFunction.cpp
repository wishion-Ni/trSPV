// CompositeLossFunction.cpp
#include "CompositeLossFunction.h"
#include <stdexcept>

void CompositeLossFunction::addLoss(std::shared_ptr<ComplexLossFunction> loss, double weight) {
    if (!loss) throw std::invalid_argument("CompositeLossFunction::addLoss - nullptr passed");
    losses.emplace_back(loss, weight);
}

double CompositeLossFunction::evaluate(const std::vector<std::complex<double>>& prediction,
    const std::vector<std::complex<double>>& target) const {
    if (prediction.size() != target.size()) throw std::invalid_argument("CompositeLossFunction::evaluate - size mismatch");
    double sum = 0.0;
    for (const auto& pair : losses) {
        const auto& loss = pair.first;
        double weight = pair.second;
        sum += weight * loss->evaluate(prediction, target);
    }
    return sum;
}

std::vector<std::complex<double>> CompositeLossFunction::gradient(
    const std::vector<std::complex<double>>& prediction,
    const std::vector<std::complex<double>>& target) const {

    if (prediction.size() != target.size()) throw std::invalid_argument("CompositeLossFunction::gradient - size mismatch");

    std::vector<std::complex<double>> grad(prediction.size(), { 0.0, 0.0 });
    for (const auto& pair : losses) {
        const auto& loss = pair.first;
        double weight = pair.second;
        auto g = loss->gradient(prediction, target);
        for (size_t i = 0; i < grad.size(); ++i) {
            grad[i] += weight * g[i];
        }
    }
    return grad;
}