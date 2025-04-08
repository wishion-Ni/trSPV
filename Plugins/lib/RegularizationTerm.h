#pragma once

#ifdef REGULARIZATION_EXPORTS
#define REG_API __declspec(dllexport)
#else
#define REG_API __declspec(dllimport)
#endif

#include <vector>
#include <complex>

class REG_API RegularizationTerm {
public:
    virtual ~RegularizationTerm() = default;

    // Evaluate regularization value
    virtual double evaluate(const std::vector<std::complex<double>>& x) const = 0;

    // Gradient of regularization w.r.t x
    virtual std::vector<std::complex<double>> gradient(const std::vector<std::complex<double>>& x) const = 0;

    virtual const char* name() const = 0;
};