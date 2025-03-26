#pragma once
#include "ErrorFunction.h"
#include <vector>
#include <cmath>
#include <string>

class L1ErrorFunction : public ErrorFunction {
public:
    double evaluate(const std::vector<double>& prediction,
        const std::vector<double>& target) const override;

    std::vector<double> gradient(const std::vector<double>& prediction,
        const std::vector<double>& target) const override;

    const char* name() const override {
        return "L1";
    }
};
