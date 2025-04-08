#include "LossLoader.h"
#include <iostream>

LossLoader::LossLoader() = default;

LossLoader::~LossLoader() {
    //if (dll_handle) FreeLibrary(dll_handle);
}

bool LossLoader::load(const std::string & dll_path) {
    dll_handle = LoadLibraryA(dll_path.c_str());
    if (!dll_handle) {
        std::cerr << "Failed to load DLL: " << dll_path << std::endl;
        return false;
    }

    create_func = reinterpret_cast<CreateFunc>(GetProcAddress(dll_handle, "CreateComplexLossFunctionFromJSON"));
    destroy_func = reinterpret_cast<DestroyFunc>(GetProcAddress(dll_handle, "DestroyComplexLossFunction"));

    if (!create_func || !destroy_func) {
        std::cerr << "Failed to get required functions from DLL." << std::endl;
        return false;
    }
    return true;
}

std::shared_ptr<ComplexLossFunction> LossLoader::createFromJSON(const std::string & json) {
    if (!create_func || !destroy_func) return nullptr;
    ComplexLossFunction* raw = create_func(json.c_str());
    if (!raw) {
        std::cerr << "Loss creation from JSON failed." << std::endl;
        return nullptr;
    }
    return std::shared_ptr<ComplexLossFunction>(raw, [this](ComplexLossFunction* p) {
        destroy_func(p);
        });
}

LossLoader& LossLoader::instance() {
    static LossLoader loader;
    return loader;
}
