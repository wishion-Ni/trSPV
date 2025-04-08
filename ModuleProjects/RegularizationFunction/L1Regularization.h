#pragma once
#include "RegularizationTerm.h"

class REG_API L1Regularization : public RegularizationTerm {
public:
    double evaluate(const std::vector<std::complex<double>>& x) const override;
    std::vector<std::complex<double>> gradient(const std::vector<std::complex<double>>& x) const override;
    const char* name() const override { return "L1"; }
};
