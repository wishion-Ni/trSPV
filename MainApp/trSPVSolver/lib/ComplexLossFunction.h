#pragma once

#ifdef LOSS_FUNCTIONS_EXPORTS
#define LOSS_API __declspec(dllexport)
#else
#define LOSS_API __declspec(dllimport)
#endif

#include <vector>
#include <complex>
#include <string>
#include <cmath>
#include <stdexcept>
#include <memory>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class LOSS_API ComplexLossFunction {
public:
    virtual ~ComplexLossFunction() = default;

    // ������ʧֵ
    virtual double evaluate(const std::vector<std::complex<double>>& prediction,
        const std::vector<std::complex<double>>& target) const = 0;

    // �����ݶȣ�������ʽ��
    virtual std::vector<std::complex<double>> gradient(const std::vector<std::complex<double>>& prediction,
        const std::vector<std::complex<double>>& target) const = 0;

    // ����
    virtual const char* name() const = 0;
};