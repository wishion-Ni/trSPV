#pragma once

#include <string>
#include <memory>
#include <vector>
#include <complex>
#include <Windows.h>

class RegularizationTerm;

class RegLoader {
public:
    RegLoader();
    ~RegLoader();

    bool load(const std::string& dll_path);

    // 构造接口
    void* create_raw_handle(const std::string& json);
    std::shared_ptr<RegularizationTerm> createFromJSON(const std::string& json);

    // 工具函数
    void set_raw_handle(void* h);
    void* raw_handle() const;

    // DLL 函数接口（用于主程序直接调用）
    using CreateFunc = void* (*)(const char*);
    using DestroyFunc = void (*)(RegularizationTerm*);
    using EvalFunc = double (*)(void*, const std::complex<double>*, std::size_t);
    using GradFunc = void (*)(void*, const std::complex<double>*, std::complex<double>*, std::size_t);
    using NameFunc = const char* (*)(void*);
    using ExtractFunc = RegularizationTerm * (*)(void*);

    EvalFunc eval_func = nullptr;
    GradFunc grad_func = nullptr;
    NameFunc name_func = nullptr;
    ExtractFunc extract_func = nullptr;
    DestroyFunc destroy_func = nullptr;

    static RegLoader& instance();

private:
    HMODULE dll_handle = nullptr;
    void* raw_handle_ = nullptr;
    CreateFunc create_func = nullptr;
};
