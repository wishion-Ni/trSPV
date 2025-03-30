#include "JSDivLoss.h"

double JSDivLoss::evaluate(const std::vector<std::complex<double>>& prediction,
    const std::vector<std::complex<double>>& target) const {
    double eps = 1e-12;
    double norm_pred = 0.0, norm_target = 0.0;
    for (size_t i = 0; i < prediction.size(); ++i) {
        norm_pred += std::norm(prediction[i]);
        norm_target += std::norm(target[i]);
    }
    double loss = 0.0;
    for (size_t i = 0; i < prediction.size(); ++i) {
        double p = std::norm(target[i]) / (norm_target + eps);
        double q = std::norm(prediction[i]) / (norm_pred + eps);
        double m = 0.5 * (p + q);
        loss += 0.5 * p * std::log((p + eps) / (m + eps)) + 0.5 * q * std::log((q + eps) / (m + eps));
    }
    return loss;
}

std::vector<std::complex<double>> JSDivLoss::gradient(const std::vector<std::complex<double>>& prediction,
    const std::vector<std::complex<double>>& target) const {
    return std::vector<std::complex<double>>(prediction.size(), { 0.0, 0.0 });
}