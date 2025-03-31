#pragma once

#include "ComplexLossFunction.h"

#ifdef __cplusplus
extern "C" {
#endif

	LOSS_API ComplexLossFunction* CreateComplexLossFunction(const char* name);
	LOSS_API void DestroyComplexLossFunction(ComplexLossFunction* ptr);

#ifdef __cplusplus
}
#endif