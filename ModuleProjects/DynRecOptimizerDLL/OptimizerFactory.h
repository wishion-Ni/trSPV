#pragma once

#include "Optimizer.h"
#include "AdamOptimizer.h"
#include "GradientDescentOptimizer.h"
#include "MomentumOptimizer.h"
#include "NAGOptimizer.h"
#include "ConjugateGradientOptimizer.h"
#include "PSOOptimizer.h"
#include "DEOptimizer.h"
#include <nlohmann/json.hpp>
#include <memory>
#include <string>

class OptimizerFactory {
public:
    static std::unique_ptr<Optimizer> create(const std::string& method, const nlohmann::json& config) {
        
        if (method == "adam") return std::make_unique<AdamOptimizer>(config);
        if (method == "sgd") return std::make_unique<GradientDescentOptimizer>(config);
        if (method == "momentum") return std::make_unique<MomentumOptimizer>(config);
        if (method == "nag") return std::make_unique<NAGOptimizer>(config);
        if (method == "cg") return std::make_unique<ConjugateGradientOptimizer>(config);
        if (method == "pso") return std::make_unique<PSOOptimizer>(config);
        if (method == "de") return std::make_unique<DEOptimizer>(config);
        if (method == "gd") return std::make_unique<GradientDescentOptimizer>(config);
        return nullptr;
    }
};