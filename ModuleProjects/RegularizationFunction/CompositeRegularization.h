#pragma once
#include "RegularizationTerm.h"

class REG_API CompositeRegularization : public RegularizationTerm {
public:
    void addTerm(std::shared_ptr<RegularizationTerm> term, double weight);

    double evaluate(const std::vector<std::complex<double>>& x) const override;
    std::vector<std::complex<double>> gradient(const std::vector<std::complex<double>>& x) const override;
    const char* name() const override { return "Composite"; }

private:
    std::vector<std::pair<std::shared_ptr<RegularizationTerm>, double>> terms_;
};