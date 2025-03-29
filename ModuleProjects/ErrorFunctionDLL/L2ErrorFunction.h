#pragma once

#include "ErrorFunction.h"
#include <vector>
#include <string>
#include <cmath>

/**
 * @brief L2 范数误差函数实现（最小二乘）
 *        E(x) = 0.5 * sum_i (prediction_i - target_i)^2
 */
class ERROR_API L2ErrorFunction : public ErrorFunction {
public:
    L2ErrorFunction() {}

    double evaluate(const std::vector<double>& prediction,
        const std::vector<double>& target) const override;

    std::vector<double> gradient(const std::vector<double>& prediction,
        const std::vector<double>& target) const override;

    const char* name() const override {
        return "L2";
    }
};

