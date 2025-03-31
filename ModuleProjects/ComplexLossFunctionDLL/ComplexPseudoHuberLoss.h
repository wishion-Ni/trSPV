#pragma once
#include "ComplexLossFunction.h"
class LOSS_API ComplexPseudoHuberLoss : public ComplexLossFunction {
public:
    ComplexPseudoHuberLoss(double delta = 1.0);

    double evaluate(const std::vector<std::complex<double>>& prediction,
        const std::vector<std::complex<double>>& target) const override;

    std::vector<std::complex<double>> gradient(const std::vector<std::complex<double>>& prediction,
        const std::vector<std::complex<double>>& target) const override;

    const char* name() const override { return "ComplexPseudoHuber"; }

private:
    double delta;
};