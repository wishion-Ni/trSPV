#include "CompositeRegularization.h"

void CompositeRegularization::addTerm(std::shared_ptr<RegularizationTerm> term, double weight) {
    if (!term) throw std::invalid_argument("CompositeRegularizationFunction::addTerm - nullptr passed");
    terms_.emplace_back(term, weight);
}

double CompositeRegularization::evaluate(const std::vector<std::complex<double>>& x) const {
    double total = 0.0;
    for (const auto& [term, weight] : terms_) {
        total += weight * term->evaluate(x);
    }
    return total;
}

std::vector<std::complex<double>> CompositeRegularization::gradient(const std::vector<std::complex<double>>& x) const {
    std::vector<std::complex<double>> total_grad(x.size(), { 0.0, 0.0 });
    for (const auto& [term, weight] : terms_) {
        auto grad = term->gradient(x);
        for (size_t i = 0; i < grad.size(); ++i) {
            total_grad[i] += weight * grad[i];
        }
    }
    return total_grad;
}