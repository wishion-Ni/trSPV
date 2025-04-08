#pragma once
#include "DynRecOptimizer.h"

extern "C" {

    DYNREC_API void* CreateDynRecOptimizer();

    DYNREC_API void DestroyDynRecOptimizer(void* handle);

    DYNREC_API bool LoadPlugins(void* handle, const char* loss_path, const char* reg_path);

    DYNREC_API bool Configure(void* handle,
        const std::complex<double>* A_data,
        int rows, int cols,
        const std::complex<double>* target_data,
        int target_len,
        const char* loss_json,
        const char* reg_json,
        double reg_weight,
        const double* x0_data,
        int x0_len);


    DYNREC_API bool Optimize(void* handle,
        const char* optimizer_name,
        const char* optimizer_config_json,
        double* out_result, int result_len);
}