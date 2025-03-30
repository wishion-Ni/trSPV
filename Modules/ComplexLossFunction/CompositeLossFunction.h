#pragma once
#include "ComplexLossFunction.h"
#include <memory>
#include <vector>
#include <utility>

class CompositeLossFunction : public ComplexLossFunction {
public:
    void addLoss(std::shared_ptr<ComplexLossFunction> loss, double weight);

    double evaluate(const std::vector<std::complex<double>>& prediction,
        const std::vector<std::complex<double>>& target) const override;

    std::vector<std::complex<double>> gradient(const std::vector<std::complex<double>>& prediction,
        const std::vector<std::complex<double>>& target) const override;

    const char* name() const override { return "CompositeLoss"; }

    size_t size() const { return losses.size(); }

private:
    std::vector<std::pair<std::shared_ptr<ComplexLossFunction>, double>> losses;
};