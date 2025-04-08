// main_test_dynrec_dll.cpp
#include <windows.h>
#include <iostream>
#include <vector>
#include <complex>
#include <string>
#include <nlohmann/json.hpp>

using Complex = std::complex<double>;

using CreateFunc = void* (*)();
using DestroyFunc = void (*)(void*);
using LoadFunc = bool (*)(void*, const char*, const char*);
using ConfigFunc = bool (*)(void*, const Complex*, int, int, const Complex*, int, const char*, const char*, double, const double*, int);
using OptimizeFunc = bool (*)(void*, const char*, const char*, double*, int);

int main() {
    HMODULE dll = LoadLibraryA("../Plugins/DynRecOptimizerDLL.dll");
    if (!dll) {
        std::cerr << "Failed to load DLL." << std::endl;
        return 1;
    }

    auto Create = (CreateFunc)GetProcAddress(dll, "CreateDynRecOptimizer");
    auto Destroy = (DestroyFunc)GetProcAddress(dll, "DestroyDynRecOptimizer");
    auto Load = (LoadFunc)GetProcAddress(dll, "LoadPlugins");
    auto Configure = (ConfigFunc)GetProcAddress(dll, "Configure");
    auto Optimize = (OptimizeFunc)GetProcAddress(dll, "Optimize");

    if (!Create || !Destroy || !Load || !Configure || !Optimize) {
        std::cerr << "Failed to get one or more function pointers." << std::endl;
        return 2;
    }

    void* handle = Create();

    // Plugin 路径
    const char* loss_dll = "../Plugins/ComplexLossFunctionDLL.dll";
    const char* reg_dll = "../Plugins/RegularizationFunction.dll";
    if (!Load(handle, loss_dll, reg_dll)) {
        std::cerr << "Failed to load plugins." << std::endl;
        return 3;
    }

    // 构造 A, target, x0
    Complex A_data[] = {
        {1.0, 0.0}, {0.5, 0.1}, {0.2, -0.3},
        {0.1, 0.2}, {1.0, 0.0}, {0.3, 0.1},
        {0.0, -0.1}, {0.4, 0.3}, {1.0, 0.0}
    };
    Complex target[] = { {1.2, 0.3}, {2.1, 0.4}, {1.8, -0.2} };
    double x0[] = { 0.0, 0.0, 0.0 };

    // JSON
    std::string loss_json = R"({"type": "AmplitudePhaseL2", "numeric": {"amp_weight": 1.0, "phase_weight": 1.0}})";
    std::string reg_json = R"({"type": "L2"})";

    bool config_ok = Configure(handle, A_data, 3, 3, target, 3, loss_json.c_str(), reg_json.c_str(), 0.01, x0, 3);
    if (!config_ok) {
        std::cerr << "Configuration failed." << std::endl;
        return 4;
    }

    // 优化器配置
    std::string optimizer_name = "pso";
    nlohmann::json optimizer_cfg = {
        {"max_iter", 1000}
    };
    std::string optimizer_cfg_str = optimizer_cfg.dump();

    double result[3];
    if (!Optimize(handle, optimizer_name.c_str(), optimizer_cfg_str.c_str(), result, 3)) {
        std::cerr << "Optimization failed." << std::endl;
        return 5;
    }

    std::cout << "Optimized x: ";
    for (double v : result) std::cout << v << " ";
    std::cout << std::endl;

    Destroy(handle);
    FreeLibrary(dll);
    return 0;
}
