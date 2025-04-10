#pragma once

#ifdef _WIN32
#define TR_DLL_API __declspec(dllexport)
#else
#define TR_DLL_API
#endif

#include <complex>
#include <string>


extern "C" {
    // 构造函数 / 析构函数
    TR_DLL_API void* CreateSelector();
    TR_DLL_API void DestroySelector(void* handle);

    // 配置接口
    TR_DLL_API void SetProblem(void* handle, const std::complex<double>* A, int m, int n,
        const std::complex<double>* b,
        const std::complex<double>* L, int l_rows, int l_cols);

    TR_DLL_API void SetLambdaLogSpace(void* handle, double lambda_min, double lambda_max, int num_points);

    // 策略设置
    TR_DLL_API void SetCriterion(void* handle, int criterion); // 0: Lcurve, 1: Validation, 2: GCV
    TR_DLL_API void SetLcurveCornerMethod(void* handle, int method); // 0: MaxCurvature, 1: Triangle

    // 验证集
    TR_DLL_API void SetValidationSet(void* handle, const std::complex<double>* A_val,
        const std::complex<double>* b_val, int m_val);

    // 主功能
    TR_DLL_API double FindBestLambda(void* handle);
    TR_DLL_API bool ExportLcurveCSV(void* handle, const char* filename);
    TR_DLL_API bool GetSolution(const double lambda, void* handle, double* x_out);
}
