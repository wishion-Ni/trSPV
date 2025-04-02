#pragma once
#include "RegularizationTerm.h"

class CompositeRegularization : public RegularizationTerm {
public:
    void addTerm(std::unique_ptr<RegularizationTerm> term, double weight);

    double evaluate(const std::vector<std::complex<double>>& x) const override;
    std::vector<std::complex<double>> gradient(const std::vector<std::complex<double>>& x) const override;
    const char* name() const override { return "Composite"; }

private:
    std::vector<std::pair<std::unique_ptr<RegularizationTerm>, double>> terms_;
};