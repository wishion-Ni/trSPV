#pragma once
#include "ErrorFunction.h"

extern "C" ERROR_API ErrorFunction * CreateErrorFunction(const char* name);
extern "C" ERROR_API void DestroyErrorFunction(ErrorFunction * ptr);