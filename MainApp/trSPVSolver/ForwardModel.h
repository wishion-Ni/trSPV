#pragma once

#include <Eigen/Dense>
#include <vector>
#include <complex>

class ForwardModel {
public:
    virtual ~ForwardModel() = default;

    virtual std::vector<std::complex<double>> apply(const std::vector<double>& x) const = 0;
    virtual std::vector<double> apply_adjoint(const std::vector<std::complex<double>>& grad_pred) const = 0;
};

class EigenForwardModel : public ForwardModel {
public:
    using MatrixXcd = Eigen::MatrixXcd;
    using VectorXcd = Eigen::VectorXcd;
    using VectorXd = Eigen::VectorXd;

    EigenForwardModel(const MatrixXcd& A) : A_(A) {}

    std::vector<std::complex<double>> apply(const std::vector<double>& x) const override {
        VectorXd x_eig = Eigen::Map<const VectorXd>(x.data(), x.size());
        VectorXcd result = A_ * x_eig;
        return std::vector<std::complex<double>>(result.data(), result.data() + result.size());
    }

    std::vector<double> apply_adjoint(const std::vector<std::complex<double>>& grad_pred) const override {
        VectorXcd g = Eigen::Map<const VectorXcd>(grad_pred.data(), grad_pred.size());
        Eigen::VectorXcd grad_cplx = A_.adjoint() * g;
        Eigen::VectorXd grad = grad_cplx.real();
        return std::vector<double>(grad.data(), grad.data() + grad.size());
    }

private:
    MatrixXcd A_;
};
