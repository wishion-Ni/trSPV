#include <iostream>
#include <Windows.h>
#include <vector>
#include "../Plugins/lib/ComplexLossFunction.h"
#include "../Plugins/lib/ComplexLossFunctionFactor.h"  // DLL�����ӿ�����

int main() {
    // ���� DLL
    HMODULE hDll = LoadLibraryA("../../Plugins/ComplexLossFunctionDLL.dll");
    if (!hDll) {
        std::cerr << "Failed to load DLL!" << std::endl;
        return -1;
    }

    // ��ȡ����ָ��
    typedef ComplexLossFunction* (*CreateFunc)(const char*);
    typedef void (*DestroyFunc)(ComplexLossFunction*);

    auto CreateComplexLossFunction = (CreateFunc)GetProcAddress(hDll, "CreateComplexLossFunction");
    auto DestroyComplexLossFunction = (DestroyFunc)GetProcAddress(hDll, "DestroyComplexLossFunction");

    if (!CreateComplexLossFunction || !DestroyComplexLossFunction) {
        std::cerr << "Function pointers not found!" << std::endl;
        FreeLibrary(hDll);
        return -2;
    }

    // ��������ʵ��
    ComplexLossFunction* error = CreateComplexLossFunction("L2");
    if (!error) {
        std::cerr << "ComplexLossFunction not created!" << std::endl;
        FreeLibrary(hDll);
        return -3;
    }

    // ���ò���
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

    // ����
    DestroyComplexLossFunction(error);
    FreeLibrary(hDll);
    return 0;
}
