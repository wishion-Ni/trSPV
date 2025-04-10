#include "DynRecOptimizerRun.h"
#include "lib/DynRecOptimizerDLL.h"
#include <windows.h>
#include <stdexcept>
#include <iostream>
#include <nlohmann/json.hpp>

std::vector<double> runOptimization(
    const OptimizationProblem& problem,
    const std::string& optimizer_config_json) {

    using CreateFunc = void* (*)();
    using DestroyFunc = void (*)(void*);
    using LoadFunc = bool (*)(void*, const char*, const char*);
    using ConfigFunc = bool (*)(void*, const std::complex<double>*, int, int, const std::complex<double>*, int, const char*, const char*, double, const double*, int);
    using OptimizeFunc = bool (*)(void*, const char*, const char*, double*, int);

    HMODULE dll = LoadLibraryA("plugins/DynRecOptimizerDLL.dll");
    if (!dll) throw std::runtime_error("[runOptimization] Failed to load DynRecOptimizerDLL.dll");

    auto Create = (CreateFunc)GetProcAddress(dll, "CreateDynRecOptimizer");
    auto Destroy = (DestroyFunc)GetProcAddress(dll, "DestroyDynRecOptimizer");
    auto Load = (LoadFunc)GetProcAddress(dll, "LoadPlugins");
    auto Configure = (ConfigFunc)GetProcAddress(dll, "Configure");
    auto Optimize = (OptimizeFunc)GetProcAddress(dll, "Optimize");

    if (!Create || !Destroy || !Load || !Configure || !Optimize)
        throw std::runtime_error("[runOptimization] Failed to bind DLL function pointers");

    void* handle = Create();
    if (!Load(handle, "plugins/ComplexLossFunctionDLL.dll", "plugins/RegularizationFunction.dll"))
        throw std::runtime_error("[runOptimization] Failed to load plugin DLLs");

    if (!Configure(handle,
        problem.A_flat.data(), problem.m, problem.n,
        problem.target.data(), problem.m,
        problem.loss_json.c_str(),
        problem.reg_json.c_str(),
        problem.reg_weight,
        problem.x0.data(), problem.n)) {
        throw std::runtime_error("[runOptimization] Configure failed");
    }

    if (optimizer_config_json.empty())
        throw std::runtime_error("[runOptimization] Optimizer config JSON is empty");

    nlohmann::json opt_cfg;
    try {
        opt_cfg = nlohmann::json::parse(optimizer_config_json);
    }
    catch (const std::exception& e) {
        throw std::runtime_error(std::string("[runOptimization] Failed to parse optimizer JSON: ") + e.what());
    }

    std::string opt_name = opt_cfg.value("type", "adam");
    std::vector<double> result(problem.n);
    if (!Optimize(handle, opt_name.c_str(), optimizer_config_json.c_str(), result.data(), problem.n))
        throw std::runtime_error("[runOptimization] Optimization failed");

    Destroy(handle);
    FreeLibrary(dll);
    return result;
}