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

    void set_param(const std::unordered_map<std::string, double>& params) override {
        auto it_amp = params.find("amp_weight");
        if (it_amp != params.end()) {
            w_amp = it_amp->second;
        }

        auto it_phase = params.find("phase_weight");
        if (it_phase != params.end()) {
            w_phase = it_phase->second;
        }
    }

private:
    double w_amp;
    double w_phase;
};