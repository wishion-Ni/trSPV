#include "RegLoader.h"
#include "../../Plugins/lib/RegularizationTermDLL.h"
#include "../../Plugins/lib/RegularizationTerm.h"
#include <iostream>

RegLoader::RegLoader() = default;

RegLoader::~RegLoader() {
    //if (dll_handle) FreeLibrary(dll_handle);
}

bool RegLoader::load(const std::string& dll_path) {
    dll_handle = LoadLibraryA(dll_path.c_str());
    if (!dll_handle) {
        std::cerr << "Failed to load regularization DLL: " << dll_path << std::endl;
        return false;
    }

    create_func = reinterpret_cast<CreateFunc>(GetProcAddress(dll_handle, "CreateRegularizationFromJSON"));
    destroy_func = reinterpret_cast<DestroyFunc>(GetProcAddress(dll_handle, "DestroyRegularization"));
    eval_func = reinterpret_cast<EvalFunc>(GetProcAddress(dll_handle, "EvaluateRegularization"));
    grad_func = reinterpret_cast<GradFunc>(GetProcAddress(dll_handle, "ComputeRegularizationGradient"));
    name_func = reinterpret_cast<NameFunc>(GetProcAddress(dll_handle, "GetRegularizationName"));
    extract_func = reinterpret_cast<ExtractFunc>(GetProcAddress(dll_handle, "GetRegularizationPointer"));

    if (!create_func || !destroy_func || !eval_func || !grad_func || !name_func || !extract_func) {
        std::cerr << "Failed to get required functions from regularization DLL." << std::endl;
        return false;
    }
    return true;
}

void* RegLoader::create_raw_handle(const std::string& json) {
    if (!create_func) return nullptr;
    void* h = create_func(json.c_str());
    raw_handle_ = h;
    return h;
}

std::shared_ptr<RegularizationTerm> RegLoader::createFromJSON(const std::string& json) {
    void* handle = create_raw_handle(json);
    if (!handle || !extract_func) {
        std::cerr << "[RegLoader] Failed to create or extract regularization." << std::endl;
        return nullptr;
    }

    RegularizationTerm* ptr = extract_func(handle);
    if (!ptr) {
        std::cerr << "[RegLoader] Extracted null RegularizationTerm*" << std::endl;
        return nullptr;
    }

    DestroyFunc destroy = destroy_func;
    return std::shared_ptr<RegularizationTerm>(ptr, [destroy](RegularizationTerm* p) {
      //  destroy(p);
        });
}

void RegLoader::set_raw_handle(void* h) {
    raw_handle_ = h;
}

void* RegLoader::raw_handle() const {
    return raw_handle_;
}

RegLoader& RegLoader::instance() {
    static RegLoader loader;
    return loader;
}