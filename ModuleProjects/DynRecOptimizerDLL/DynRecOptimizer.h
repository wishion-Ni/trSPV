#pragma once

#ifdef DYNREC_OPTIMIZER_DLL_EXPORTS
#define DYNREC_API __declspec(dllexport)
#else
#define DYNREC_API __declspec(dllimport)
#endif


#include <string>
#include <vector>
#include <complex>
#include <memory>
#include <Eigen/Dense>
#include <nlohmann/json.hpp>
#include "DynRecLogger.h"

class ComplexLossFunction;
class ForwardModel;

class DYNREC_API DynRecOptimizer {
public:
    // 加载损失函数和正则项的 DLL
    bool loadPlugins(const std::string& loss_dll_path, const std::string& reg_dll_path);

    // 配置模型矩阵、目标数据、损失函数、正则项及正则权重
    bool configure(const Eigen::MatrixXcd& A,
        const std::vector<std::complex<double>>& target,
        const std::string& loss_json,
        const std::string& reg_json,
        double reg_weight,
        const std::vector<double>& x0 = {});

    // 根据优化器名和配置运行优化，返回优化后的 x
    std::vector<double> optimize(const std::string& method, const nlohmann::json& optimizer_config);

private:
    std::shared_ptr<ForwardModel> model_;
    std::shared_ptr<ComplexLossFunction> loss_;
    void* reg_handle_ = nullptr;
    std::vector<std::complex<double>> target_;
    std::vector<double> x0_;
    double reg_weight_ = 0.0;

    nlohmann::json loss_config_;
    nlohmann::json reg_config_;

    DynRecLogger logger_;
};
