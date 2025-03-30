#pragma once

#include "ErrorFunction.h"

#ifdef __cplusplus
extern "C" {
#endif

	ERROR_API ErrorFunction* CreateErrorFunction(const char* name);
	ERROR_API void DestroyErrorFunction(ErrorFunction* ptr);

#ifdef __cplusplus
}
#endif
