#pragma once
#include "RegularizationTerm.h"

#ifdef __cplusplus
extern "C" {
#endif

	REG_API void* CreateRegularizationFromJSON(const char* json_config);
	REG_API void DestroyRegularization(void* handle);
	REG_API double EvaluateRegularization(void* handle, const std::complex<double>* x, std::size_t n);
	REG_API void ComputeRegularizationGradient(void* handle, const std::complex<double>* x, std::complex<double>* grad_out, std::size_t n);
	REG_API const char* GetRegularizationName(void* handle);
	REG_API RegularizationTerm* GetRegularizationPointer(void* handle);

#ifdef __cplusplus
}
#endif