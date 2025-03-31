#include "CosineSimilarityLoss.h"

double CosineSimilarityLoss::evaluate(const std::vector<std::complex<double>>& prediction,
    const std::vector<std::complex<double>>& target) const {
    if (prediction.size() != target.size()) throw std::invalid_argument("CosSim: size mismatch");
    std::complex<double> dot = 0.0;
    double norm_p = 0.0, norm_t = 0.0;
    for (size_t i = 0; i < prediction.size(); ++i) {
        dot += std::conj(target[i]) * prediction[i];
        norm_p += std::norm(prediction[i]);
        norm_t += std::norm(target[i]);
    }
    if (norm_p == 0 || norm_t == 0) return 1.0;
    return 1.0 - std::real(dot) / (std::sqrt(norm_p * norm_t));
}

std::vector<std::complex<double>> CosineSimilarityLoss::gradient(const std::vector<std::complex<double>>& prediction,
    const std::vector<std::complex<double>>& target) const {
    // 暂不实现梯度（通常用作评估指标）
    return std::vector<std::complex<double>>(prediction.size(), { 0.0, 0.0 });
}