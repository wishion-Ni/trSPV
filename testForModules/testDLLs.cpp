#include <Windows.h>
#include <iostream>
#include <vector>
#include <complex>
#include "../Plugins/lib/ComplexLossFunction.h"

// 接口函数指针定义（对应 extern "C" 的 DLL 导出函数）
typedef ComplexLossFunction* (*CreateFunc)(const char*);
typedef void (*DestroyFunc)(ComplexLossFunction*);

int main() {
    HMODULE dll = LoadLibraryA("../Plugins/ComplexLossFunctionDLL.dll");
    if (!dll) {
        std::cerr << "Failed to load DLL!" << std::endl;
        return 1;
    }

    auto create = (CreateFunc)GetProcAddress(dll, "CreateComplexLossFunctionFromJSON");
    auto destroy = (DestroyFunc)GetProcAddress(dll, "DestroyComplexLossFunction");

    if (!create || !destroy) {
        std::cerr << "Failed to load function symbols." << std::endl;
        FreeLibrary(dll);
        return 1;
    }

    const char* json_config = R"({
        "type": "Composite",
        "composite": [
            { "type": "L2", "weight": 0.6 },
            { "type": "KLDiv", "weight": 0.4 }
        ]
    })";

    ComplexLossFunction* loss = create(json_config);
    if (!loss) {
        std::cerr << "Failed to create loss function." << std::endl;
        FreeLibrary(dll);
        return 1;
    }

    std::vector<std::complex<double>> pred = { {1.0, 0.5}, {0.8, -0.2}, {0.3, 0.9} };
    std::vector<std::complex<double>> target = { {1.1, 0.6}, {0.7, -0.1}, {0.4, 1.0} };

    double val = loss->evaluate(pred, target);
    auto grad = loss->gradient(pred, target);

    std::cout << "Loss value: " << val << std::endl;
    for (const auto& g : grad) {
        std::cout << g << std::endl;
    }

    destroy(loss);
    FreeLibrary(dll);
    return 0;
}
