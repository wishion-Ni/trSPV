#include "RegularizationTermDLL.h"
#include "RegTermRegistry.h"
#include "ParseRegConfigFromJSON.h"       
#include "RegisterAllRegularizationTerms.h" 
#include <complex>
#include <string>

namespace {
    class RegularizationHandle {
    public:
        std::unique_ptr<RegularizationTerm> term;
    };
}

extern "C" {

    REG_API void* CreateRegularizationFromJSON(const char* json_config) {
        static bool initialized = false;
        if (!initialized) {
            RegisterAllRegularizationTerms(RegTermRegistry::instance());
            initialized = true;
        }

        RegConfig config;
        if (!ParseRegConfigFromJSON(json_config, config)) {
            return nullptr;
        }

        auto term = RegTermRegistry::instance().create(config);
        if (!term) return nullptr;

        auto* handle = new RegularizationHandle();
        handle->term = std::move(term);
        return handle;
    }

    REG_API void DestroyRegularization(void* handle) {
        delete static_cast<RegularizationHandle*>(handle);
    }

    REG_API double EvaluateRegularization(void* handle, const std::complex<double>* x, std::size_t n) {
        if (!handle) return 0.0;
        auto* h = static_cast<RegularizationHandle*>(handle);
        std::vector<std::complex<double>> vec(x, x + n);
        return h->term->evaluate(vec);
    }

    REG_API void ComputeRegularizationGradient(void* handle, const std::complex<double>* x, std::complex<double>* grad_out, std::size_t n) {
        if (!handle) return;
        auto* h = static_cast<RegularizationHandle*>(handle);
        std::vector<std::complex<double>> vec(x, x + n);
        auto grad = h->term->gradient(vec);
        for (std::size_t i = 0; i < n; ++i) {
            grad_out[i] = grad[i];
        }
    }

    REG_API const char* GetRegularizationName(void* handle) {
        if (!handle) return "(null)";
        auto* h = static_cast<RegularizationHandle*>(handle);
        return h->term->name();
    }

}