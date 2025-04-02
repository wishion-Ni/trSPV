#pragma once
#include "RegularizationTerm.h"

class TVRegularization : public RegularizationTerm {
public:
    double evaluate(const std::vector<std::complex<double>>& x) const override;
    std::vector<std::complex<double>> gradient(const std::vector<std::complex<double>>& x) const override;
    const char* name() const override { return "TV"; }
};