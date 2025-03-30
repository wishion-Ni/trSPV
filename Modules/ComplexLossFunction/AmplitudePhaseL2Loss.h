#pragma once
#include "ComplexLossFunction.h"
class AmplitudePhaseL2Loss : public ComplexLossFunction {
public:
    AmplitudePhaseL2Loss(double amp_weight = 1.0, double phase_weight = 1.0)
        : w_amp(amp_weight), w_phase(phase_weight) {}

    double evaluate(const std::vector<std::complex<double>>& prediction,
        const std::vector<std::complex<double>>& target) const override;

    std::vector<std::complex<double>> gradient(const std::vector<std::complex<double>>& prediction,
        const std::vector<std::complex<double>>& target) const override;

    const char* name() const override { return "AmplitudePhaseL2"; }

private:
    double w_amp;
    double w_phase;
};