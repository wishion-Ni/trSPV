#pragma once
#include "ErrorFunction.h"
#include <vector>
#include <cmath>

class ERROR_API LogCoshErrorFunction : public ErrorFunction {
public:
    LogCoshErrorFunction() {}

    double evaluate(const std::vector<double>& prediction,
        const std::vector<double>& target) const override;

    std::vector<double> gradient(const std::vector<double>& prediction,
        const std::vector<double>& target) const override;

    const char* name() const override {
        return "LogCosh";
    }
};