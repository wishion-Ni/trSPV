#include <iostream>
#include <Windows.h>
#include <vector>
#include "../Plugins/lib/ComplexLossFunction.h"
#include "../Plugins/lib/ComplexLossFunctionFactor.h"  // DLL导出接口声明

int main() {
    // 加载 DLL
    HMODULE hDll = LoadLibraryA("../../Plugins/ComplexLossFunctionDLL.dll");
    if (!hDll) {
        std::cerr << "Failed to load DLL!" << std::endl;
        return -1;
    }

    // 获取函数指针
    typedef ComplexLossFunction* (*CreateFunc)(const char*);
    typedef void (*DestroyFunc)(ComplexLossFunction*);

    auto CreateComplexLossFunction = (CreateFunc)GetProcAddress(hDll, "CreateComplexLossFunction");
    auto DestroyComplexLossFunction = (DestroyFunc)GetProcAddress(hDll, "DestroyComplexLossFunction");

    if (!CreateComplexLossFunction || !DestroyComplexLossFunction) {
        std::cerr << "Function pointers not found!" << std::endl;
        FreeLibrary(hDll);
        return -2;
    }

    // 创建误差函数实例
    ComplexLossFunction* error = CreateComplexLossFunction("L2");
    if (!error) {
        std::cerr << "ComplexLossFunction not created!" << std::endl;
        FreeLibrary(hDll);
        return -3;
    }

    // 调用测试
    std::vector<std::complex<double>> pred = {
        {1.0, 2.0}, {0.5, -0.3}, {0.2, 0.1}
    };
    std::vector<std::complex<double>> target = {
        {1.5, 1.8}, {0.4, -0.1}, {0.0, 0.0}
    };

    std::cout << "Using ComplexLossFunction: " << error->name() << std::endl;
    std::cout << "  Value: " << error->evaluate(pred, target) << std::endl;

    auto grad = error->gradient(pred, target);
    std::cout << "  Gradient: [ ";
    for (auto g : grad) std::cout << g << " ";
    std::cout << "]" << std::endl;

    // 清理
    DestroyComplexLossFunction(error);
    FreeLibrary(hDll);
    return 0;
}
