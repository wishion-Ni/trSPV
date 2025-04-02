#pragma once
#include "RegularizationTerm.h"

class HuberRegularization : public RegularizationTerm {
public:
    explicit HuberRegularization(double delta = 1.0) : delta_(delta) {}

    double evaluate(const std::vector<std::complex<double>>& x) const override;
    std::vector<std::complex<double>> gradient(const std::vector<std::complex<double>>& x) const override;
    const char* name() const override { return "Huber"; }

private:
    double delta_;
};
