#include "DynRecOptimizerDLL.h"
#include "DynRecOptimizer.h"
#include <nlohmann/json.hpp>
#include <cstring>

extern "C" {

    DYNREC_API void* CreateDynRecOptimizer() {
        return new DynRecOptimizer();
    }

    DYNREC_API void DestroyDynRecOptimizer(void* handle) {
        delete static_cast<DynRecOptimizer*>(handle);
    }

    DYNREC_API bool LoadPlugins(void* handle, const char* loss_path, const char* reg_path) {
        auto* opt = static_cast<DynRecOptimizer*>(handle);
        return opt->loadPlugins(loss_path, reg_path);
    }

    DYNREC_API bool Configure(void* handle,
        const std::complex<double>* A_data,
        int rows, int cols,
        const std::complex<double>* target_data,
        int target_len,
        const char* loss_json,
        const char* reg_json,
        double reg_weight,
        const double* x0_data,
        int x0_len) {
        auto* opt = static_cast<DynRecOptimizer*>(handle);

        Eigen::MatrixXcd A(rows, cols);
        for (int i = 0; i < rows * cols; ++i) {
            A(i) = A_data[i];
        }

        std::vector<std::complex<double>> target(target_data, target_data + target_len);
        std::vector<double> x0;
        if (x0_data && x0_len > 0) x0 = std::vector<double>(x0_data, x0_data + x0_len);

        return opt->configure(A, target, loss_json, reg_json, reg_weight, x0);
    }

    DYNREC_API bool Optimize(void* handle,
        const char* optimizer_name,
        const char* optimizer_config_json,
        double* out_x,
        int x_len) {
        auto* opt = static_cast<DynRecOptimizer*>(handle);
        nlohmann::json cfg = nlohmann::json::parse(optimizer_config_json);
        std::vector<double> result = opt->optimize(optimizer_name, cfg);

        if (result.size() != static_cast<size_t>(x_len)) return false;
        std::memcpy(out_x, result.data(), x_len * sizeof(double));
        return true;
    }

}
