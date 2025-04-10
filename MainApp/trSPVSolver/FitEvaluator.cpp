#include "FitEvaluator.h"
#include <cmath>
#include <numeric>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

FitMetrics FitEvaluator::evaluate(const std::vector<std::complex<double>>& predicted,
    const std::vector<std::complex<double>>& measured) {
    FitMetrics metrics;
    const size_t N = predicted.size();
    if (N == 0 || measured.size() != N) return metrics;

    double rss = 0.0, amp_err_sum = 0.0, phase_err_sum = 0.0;

    for (size_t i = 0; i < N; ++i) {
        std::complex<double> diff = predicted[i] - measured[i];
        rss += std::norm(diff);

        double amp_pred = std::abs(predicted[i]);
        double amp_meas = std::abs(measured[i]);
        amp_err_sum += std::abs(amp_pred - amp_meas);

        double phase_pred = std::arg(predicted[i]);
        double phase_meas = std::arg(measured[i]);
        double dphi = (phase_pred - phase_meas) * 180.0 / M_PI;
        phase_err_sum += std::abs(dphi);
    }

    metrics.rss = rss;
    metrics.rmse = std::sqrt(rss / N);
    metrics.amplitude_error = amp_err_sum / N;
    metrics.phase_error_deg = phase_err_sum / N;
    return metrics;
}