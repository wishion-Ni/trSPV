#pragma once
#include "ComplexLossFunction.h"
class ComplexPseudoHuberLoss : public ComplexLossFunction {
public:
    ComplexPseudoHuberLoss(double delta = 1.0);

    double evaluate(const std::vector<std::complex<double>>& prediction,
        const std::vector<std::complex<double>>& target) const override;

    std::vector<std::complex<double>> gradient(const std::vector<std::complex<double>>& prediction,
        const std::vector<std::complex<double>>& target) const override;

    const char* name() const override { return "ComplexPseudoHuber"; }

    void set_param(const std::unordered_map<std::string, double>& params) override {
        auto it = params.find("delta");
        if (it != params.end()) {
            delta = it->second;
        }
    }

private:
    double delta;
};