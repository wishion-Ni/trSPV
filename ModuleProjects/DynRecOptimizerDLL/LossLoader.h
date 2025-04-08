#pragma once

#include <string>
#include <memory>
#include <Windows.h>
#include "../../Plugins/lib/ComplexLossFunction.h"

class ComplexLossFunction;

class LossLoader {
public:
    LossLoader();
    ~LossLoader();

    bool load(const std::string& dll_path);
    std::shared_ptr<ComplexLossFunction> createFromJSON(const std::string& json);

    bool isLoaded() const { return create_func && destroy_func; }

    static LossLoader& instance();

private:
    HMODULE dll_handle = nullptr;

    using CreateFunc = ComplexLossFunction * (*)(const char*);
    using DestroyFunc = void (*)(ComplexLossFunction*);

    CreateFunc create_func = nullptr;
    DestroyFunc destroy_func = nullptr;
};
