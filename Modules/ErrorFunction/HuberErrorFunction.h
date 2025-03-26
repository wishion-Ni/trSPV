#pragma once
#include "ErrorFunction.h"
#include <vector>
#include <cmath>

class HuberErrorFunction : public ErrorFunction {
public:
    explicit HuberErrorFunction(double delta = 1.0) : delta_(delta) {}

    double evaluate(const std::vector<double>& prediction,
        const std::vector<double>& target) const override;

    std::vector<double> gradient(const std::vector<double>& prediction,
        const std::vector<double>& target) const override;

    const char* name() const override {
        return "Huber";
    }

private:
    double delta_;
};