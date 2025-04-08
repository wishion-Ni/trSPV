#pragma once
#include <memory>
#include <string>
#include "ComplexLossFunction.h"
#include "RegularizerFunction.h"
#include "BaseOptimizer.h"
#include "LossConfig.h"

class LossFactory {
public:
    static std::shared_ptr<ComplexLossFunction> create(const std::string& name);
    static std::shared_ptr<ComplexLossFunction> create(const LossConfig& config);
};

class RegularizerFactory {
public:
    static std::shared_ptr<RegularizerFunction> create(const std::string& name);
};

class OptimizerFactory {
public:
    static std::shared_ptr<BaseOptimizer> create(const std::string& name);
};
