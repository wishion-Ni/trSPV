// main.cpp - 显式加载 DLL 调用优化器主程序
#include "FitConfigManager.h"
#include "DataReader.h"
#include "TauGridGenerator.h"
#include "DRTForwardModel.h"
#include <windows.h>
#include <iostream>
#include <nlohmann/json.hpp>

using Complex = std::complex<double>;

int main(int argc, char* argv[]) {
    try {
        // === 1. 加载配置 ===
        FitConfigManager config;
        if (!config.loadFromFile("config.json")) {
            std::cerr << "Failed to load config.json" << std::endl;
            return 1;
        }

        // === 2. 加载实验数据 ===
        std::vector<double> periods;
        std::vector<Complex> spv;
        if (!DataReader::readCSV(config.getDataFilePath(), periods, spv)) {
            std::cerr << "Failed to read input data" << std::endl;
            return 2;
        }

        // === 3. 生成 tau 网格 ===
        auto tau = TauGridGenerator::generateAuto(config.getTauType(), periods, config.getTauNumPoints());

        // === 4. 构建 A 矩阵模型 ===
        DRTForwardModel model(tau, periods);
        const auto& A = model.getMatrix();

        const int m = static_cast<int>(spv.size());
        const int n = static_cast<int>(tau.size());

        std::vector<Complex> A_flat(m * n);
        for (int i = 0; i < m; ++i)
            for (int j = 0; j < n; ++j)
                A_flat[i * n + j] = A(i, j);

        std::vector<double> x0(n, 0.0);

        // === 5. 加载 DLL 动态库 ===
        HMODULE dll = LoadLibraryA("plugins/DynRecOptimizerDLL.dll");
        if (!dll) {
            std::cerr << "Failed to load DLL." << std::endl;
            return 3;
        }

        using CreateFunc = void* (*)();
        using DestroyFunc = void (*)(void*);
        using LoadFunc = bool (*)(void*, const char*, const char*);
        using ConfigFunc = bool (*)(void*, const Complex*, int, int, const Complex*, int, const char*, const char*, double, const double*, int);
        using OptimizeFunc = bool (*)(void*, const char*, const char*, double*, int);

        auto Create = (CreateFunc)GetProcAddress(dll, "CreateDynRecOptimizer");
        auto Destroy = (DestroyFunc)GetProcAddress(dll, "DestroyDynRecOptimizer");
        auto Load = (LoadFunc)GetProcAddress(dll, "LoadPlugins");
        auto Configure = (ConfigFunc)GetProcAddress(dll, "Configure");
        auto Optimize = (OptimizeFunc)GetProcAddress(dll, "Optimize");

        if (!Create || !Destroy || !Load || !Configure || !Optimize) {
            std::cerr << "Failed to get one or more function pointers." << std::endl;
            return 4;
        }

        void* handle = Create();
        if (!Load(handle, "plugins/ComplexLossFunctionDLL.dll", "plugins/RegularizationFunction.dll")) {
            std::cerr << "Failed to load plugin DLLs." << std::endl;
            return 5;
        }

        if (!Configure(handle,
            A_flat.data(), m, n,
            spv.data(), m,
            config.getLossConfigJSON().c_str(),
            config.getRegConfigJSON().c_str(),
            config.getLambda(),
            x0.data(), n)) {
            std::cerr << "Configure failed." << std::endl;
            return 6;
        }

        nlohmann::json opt_json = nlohmann::json::parse(config.getOptimizerConfigJSON());
        std::string opt_name = opt_json.value("type", "adam");

        std::vector<double> result(n);
        if (!Optimize(handle, opt_name.c_str(), config.getOptimizerConfigJSON().c_str(), result.data(), n)) {
            std::cerr << "Optimize failed." << std::endl;
            return 7;
        }

        Destroy(handle);
        FreeLibrary(dll);

        // === 6. 输出结果 ===
        std::cout << "Optimized x: ";
        for (double x : result) std::cout << x << " ";
        std::cout << std::endl;

        return 0;

    }
    catch (const std::exception& e) {
        std::cerr << "[Error] " << e.what() << std::endl;
        return -1;
    }
}