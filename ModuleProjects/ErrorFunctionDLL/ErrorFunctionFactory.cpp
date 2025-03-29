#include "ErrorFunctionFactory.h"
#include "L2ErrorFunction.h"
#include "L1ErrorFunction.h"
#include "HuberErrorFunction.h"
#include "LogCoshErrorFunction.h"
#include <string>

ErrorFunction* CreateErrorFunction(const char* name) {
    std::string n(name);
    if (n == "L2") return new L2ErrorFunction();
    if (n == "L1") return new L1ErrorFunction();
    if (n == "Huber") return new HuberErrorFunction(1.0);
    if (n == "LogCosh") return new LogCoshErrorFunction();
    return nullptr;
}

void DestroyErrorFunction(ErrorFunction* ptr) {
    delete ptr;
}