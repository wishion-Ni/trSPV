#include <iostream>
#include <Windows.h>
#include <vector>
#include "../Plugins/lib/ErrorFunction.h"
#include "../Plugins/lib/ErrorFunctionFactory.h"  // DLL导出接口声明

int main() {
    // 加载 DLL
    HMODULE hDll = LoadLibraryA("../../Plugins/ErrorFunctionDLL.dll");
    if (!hDll) {
        std::cerr << "Failed to load DLL!" << std::endl;
        return -1;
    }

    // 获取函数指针
    typedef ErrorFunction* (*CreateFunc)(const char*);
    typedef void (*DestroyFunc)(ErrorFunction*);

    auto CreateErrorFunction = (CreateFunc)GetProcAddress(hDll, "CreateErrorFunction");
    auto DestroyErrorFunction = (DestroyFunc)GetProcAddress(hDll, "DestroyErrorFunction");

    if (!CreateErrorFunction || !DestroyErrorFunction) {
        std::cerr << "Function pointers not found!" << std::endl;
        FreeLibrary(hDll);
        return -2;
    }

    // 创建误差函数实例
    ErrorFunction* error = CreateErrorFunction("L2");
    if (!error) {
        std::cerr << "ErrorFunction not created!" << std::endl;
        FreeLibrary(hDll);
        return -3;
    }

    // 调用测试
    std::vector<double> pred = { 1.0, 2.0, 3.0 };
    std::vector<double> targ = { 1.2, 2.1, 2.9 };

    std::cout << "Using ErrorFunction: " << error->name() << std::endl;
    std::cout << "  Value: " << error->evaluate(pred, targ) << std::endl;

    auto grad = error->gradient(pred, targ);
    std::cout << "  Gradient: [ ";
    for (auto g : grad) std::cout << g << " ";
    std::cout << "]" << std::endl;

    // 清理
    DestroyErrorFunction(error);
    FreeLibrary(hDll);
    return 0;
}
