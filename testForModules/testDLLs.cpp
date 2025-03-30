#include <iostream>
#include <Windows.h>
#include <vector>
#include "../Plugins/lib/ErrorFunction.h"
#include "../Plugins/lib/ErrorFunctionFactory.h"  // DLL�����ӿ�����

int main() {
    // ���� DLL
    HMODULE hDll = LoadLibraryA("../../Plugins/ErrorFunctionDLL.dll");
    if (!hDll) {
        std::cerr << "Failed to load DLL!" << std::endl;
        return -1;
    }

    // ��ȡ����ָ��
    typedef ErrorFunction* (*CreateFunc)(const char*);
    typedef void (*DestroyFunc)(ErrorFunction*);

    auto CreateErrorFunction = (CreateFunc)GetProcAddress(hDll, "CreateErrorFunction");
    auto DestroyErrorFunction = (DestroyFunc)GetProcAddress(hDll, "DestroyErrorFunction");

    if (!CreateErrorFunction || !DestroyErrorFunction) {
        std::cerr << "Function pointers not found!" << std::endl;
        FreeLibrary(hDll);
        return -2;
    }

    // ��������ʵ��
    ErrorFunction* error = CreateErrorFunction("L2");
    if (!error) {
        std::cerr << "ErrorFunction not created!" << std::endl;
        FreeLibrary(hDll);
        return -3;
    }

    // ���ò���
    std::vector<double> pred = { 1.0, 2.0, 3.0 };
    std::vector<double> targ = { 1.2, 2.1, 2.9 };

    std::cout << "Using ErrorFunction: " << error->name() << std::endl;
    std::cout << "  Value: " << error->evaluate(pred, targ) << std::endl;

    auto grad = error->gradient(pred, targ);
    std::cout << "  Gradient: [ ";
    for (auto g : grad) std::cout << g << " ";
    std::cout << "]" << std::endl;

    // ����
    DestroyErrorFunction(error);
    FreeLibrary(hDll);
    return 0;
}
