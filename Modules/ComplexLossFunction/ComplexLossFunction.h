#pragma once
#include <vector>
#include <complex>
#include <string>
#include <cmath>
#include <stdexcept>
#include <memory>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class ComplexLossFunction {
public:
    virtual ~ComplexLossFunction() = default;

    // 计算损失值
    virtual double evaluate(const std::vector<std::complex<double>>& prediction,
        const std::vector<std::complex<double>>& target) const = 0;

    // 计算梯度（复数形式）
    virtual std::vector<std::complex<double>> gradient(const std::vector<std::complex<double>>& prediction,
        const std::vector<std::complex<double>>& target) const = 0;

    // 名称
    virtual const char* name() const = 0;
};