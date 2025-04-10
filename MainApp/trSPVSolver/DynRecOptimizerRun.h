#pragma once

#include "OptimizationProblem.h"
#include <string>
#include <vector>

std::vector<double> runOptimization(
    const OptimizationProblem& problem,
    const std::string& optimizer_config_json);