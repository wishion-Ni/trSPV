#include <windows.h>
#include <iostream>
#include <complex>
#include <vector>
#include <string>

using Complex = std::complex<double>;

// 函数指针声明
using CreateFunc = void* (*)(const char*);
using DestroyFunc = void (*)(void*);
using EvalFunc = double (*)(void*, const Complex*, std::size_t);
using GradFunc = void (*)(void*, const Complex*, Complex*, std::size_t);
using NameFunc = const char* (*)(void*);

int main() {
    // 加载 DLL
    HMODULE hDll = LoadLibraryA("../Plugins/RegularizationFunction.dll");
    if (!hDll) {
        std::cerr << "Failed to load RegularizationFunction.dll" << std::endl;
        return 1;
    }

    // 获取函数指针
    auto Create = (CreateFunc)GetProcAddress(hDll, "CreateRegularizationFromJSON");
    auto Destroy = (DestroyFunc)GetProcAddress(hDll, "DestroyRegularization");
    auto Evaluate = (EvalFunc)GetProcAddress(hDll, "EvaluateRegularization");
    auto Gradient = (GradFunc)GetProcAddress(hDll, "ComputeRegularizationGradient");
    auto GetName = (NameFunc)GetProcAddress(hDll, "GetRegularizationName");

    if (!Create || !Destroy || !Evaluate || !Gradient || !GetName) {
        std::cerr << "Failed to bind some exported functions." << std::endl;
        FreeLibrary(hDll);
        return 1;
    }

    // JSON 配置
    std::string json_config = R"({
        "type": "Composite",
        "composite": [
            { "type": "L2", "weight": 1.0 },
            { "type": "TV", "weight": 0.5 }
        ]
    })";

    // 创建正则项
    void* handle = Create(json_config.c_str());
    if (!handle) {
        std::cerr << "Failed to create regularization term." << std::endl;
        FreeLibrary(hDll);
        return 1;
    }

    // 准备输入
    std::vector<Complex> x = { {1.0, 0.0}, {2.0, 0.5}, {1.5, -0.3} };
    std::vector<Complex> grad(x.size());

    // 评估
    double value = Evaluate(handle, x.data(), x.size());
    Gradient(handle, x.data(), grad.data(), x.size());

    std::cout << "Regularization Name: " << GetName(handle) << "\n";
    std::cout << "Value = " << value << "\n";
    std::cout << "Gradient:\n";
    for (auto& g : grad) {
        std::cout << "  " << g << "\n";
    }

    Destroy(handle);
    FreeLibrary(hDll);
    return 0;
}
