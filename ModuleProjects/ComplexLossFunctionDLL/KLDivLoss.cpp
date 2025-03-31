#include "KLDivLoss.h"

double KLDivLoss::evaluate(const std::vector<std::complex<double>>& prediction,
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
        loss += p * std::log((p + eps) / (q + eps));
    }
    return loss;
}

std::vector<std::complex<double>> KLDivLoss::gradient(const std::vector<std::complex<double>>& prediction,
    const std::vector<std::complex<double>>& target) const {
    // 常用作评估指标，暂不实现梯度
    return std::vector<std::complex<double>>(prediction.size(), { 0.0, 0.0 });
}