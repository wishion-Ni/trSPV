#pragma once
#include "ComplexLossFunction.h"
#include "LossConfig.h"
#include <functional>
#include <unordered_map>
#include <memory>

using LossBuilder = std::function<std::unique_ptr<ComplexLossFunction>(const LossConfig&, int)>;

class LossRegistry {
public:
    static LossRegistry& instance();
    void registerBuilder(const std::string& name, LossBuilder builder);
    std::unique_ptr<ComplexLossFunction> create(const LossConfig& config,int depth = 0);

private:
    std::unordered_map<std::string, LossBuilder> builders_;
};