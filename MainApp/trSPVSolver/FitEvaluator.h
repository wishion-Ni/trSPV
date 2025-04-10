#pragma once

#include <vector>
#include <complex>

struct FitMetrics {
    double rss = 0.0;
    double rmse = 0.0;
    double amplitude_error = 0.0;
    double phase_error_deg = 0.0;
};

class FitEvaluator {
public:
    static FitMetrics evaluate(const std::vector<std::complex<double>>& predicted,
        const std::vector<std::complex<double>>& measured);
};