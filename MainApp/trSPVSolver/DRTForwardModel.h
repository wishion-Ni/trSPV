#pragma once

#include "ForwardModel.h"
#include <Eigen/Dense>
#include <vector>
#include <complex>

// 可选核函数类型（可扩展）
enum class KernelType {
    Default,       // 即 DRT 响应核函数
    Exponential,   // 可留作未来备用
    Lorentzian     // 同上
};

class DRTForwardModel : public ForwardModel {
public:
    using MatrixXcd = Eigen::MatrixXcd;
    using VectorXd = Eigen::VectorXd;
    using VectorXcd = Eigen::VectorXcd;

    DRTForwardModel(const std::vector<double>& tau,
        const std::vector<double>& freq_or_period,
        KernelType kernel = KernelType::Default);

    std::vector<std::complex<double>> apply(const std::vector<double>& x) const override;
    std::vector<double> apply_adjoint(const std::vector<std::complex<double>>& grad_pred) const override;

    const MatrixXcd& getMatrix() const;

private:
    void buildMatrix();

    std::vector<double> tau_;
    std::vector<double> freq_;
    KernelType kernel_;
    MatrixXcd A_;
    double duty_ratio_ = 0.5;
};