#include "AmplitudePhaseL2Loss.h"

double AmplitudePhaseL2Loss::evaluate(const std::vector<std::complex<double>>& prediction,
    const std::vector<std::complex<double>>& target) const {
    if (prediction.size() != target.size()) throw std::invalid_argument("AmplitudePhaseL2: size mismatch");
    double sum = 0.0;
    for (size_t i = 0; i < prediction.size(); ++i) {
        double amp_diff = std::abs(prediction[i]) - std::abs(target[i]);
        double phase_diff = std::arg(prediction[i]) - std::arg(target[i]);
        if (phase_diff > M_PI) phase_diff -= 2 * M_PI;
        if (phase_diff < -M_PI) phase_diff += 2 * M_PI;
        sum += w_amp * amp_diff * amp_diff + w_phase * phase_diff * phase_diff;
    }
    return sum;
}

std::vector<std::complex<double>> AmplitudePhaseL2Loss::gradient(const std::vector<std::complex<double>>& prediction,
    const std::vector<std::complex<double>>& target) const {
    std::vector<std::complex<double>> grad(prediction.size());
    for (size_t i = 0; i < prediction.size(); ++i) {
        std::complex<double> z = prediction[i];
        std::complex<double> t = target[i];
        double r = std::abs(z), r_t = std::abs(t);
        double theta = std::arg(z), theta_t = std::arg(t);
        double d_amp = w_amp * 2.0 * (r - r_t);
        double d_phase = w_phase * 2.0 * (theta - theta_t);
        if (d_phase > M_PI) d_phase -= 2 * M_PI;
        if (d_phase < -M_PI) d_phase += 2 * M_PI;
        grad[i] = (r > 1e-10 ? d_amp * z / r : std::complex<double>(0, 0)) +
            std::complex<double>(0, d_phase);
    }
    return grad;
}