#include "DRTForwardModel.h"
#include <cmath>
#include <stdexcept>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;
using namespace Eigen;

DRTForwardModel::DRTForwardModel(const std::vector<double>& tau,
    const std::vector<double>& freq_or_period,
    KernelType kernel)
    : tau_(tau), freq_(freq_or_period), kernel_(kernel), duty_ratio_(0.5) {
    buildMatrix();
}

void DRTForwardModel::buildMatrix() {
    const int m = static_cast<int>(freq_.size());
    const int n = static_cast<int>(tau_.size());
    A_ = MatrixXcd(m, n);

    for (int i = 0; i < m; ++i) {
        double T = freq_[i];                    // ÖÜÆÚ T_k
        double w = 2.0 * M_PI / T;              // ½ÇÆµÂÊ omega_k
        double DT = duty_ratio_ * T;            // DT_k = D * T_k

        for (int j = 0; j < n; ++j) {
            double tau = tau_[j];
            double exp_num = 1.0 - exp(-DT / tau);
            double exp_den = 1.0 - exp(-T / tau);
            complex<double> phase_term = 1.0 / (1.0 + complex<double>(0.0, w * tau));
            A_(i, j) = (exp_num / exp_den) * phase_term;
        }
    }
}

std::vector<std::complex<double>> DRTForwardModel::apply(const std::vector<double>& x) const {
    VectorXd x_vec = Map<const VectorXd>(x.data(), x.size());
    VectorXcd y = A_ * x_vec.cast<std::complex<double>>();
    return std::vector<std::complex<double>>(y.data(), y.data() + y.size());
}

std::vector<double> DRTForwardModel::apply_adjoint(const std::vector<std::complex<double>>& grad_pred) const {
    VectorXcd g = Map<const VectorXcd>(grad_pred.data(), grad_pred.size());
    VectorXd grad = (A_.adjoint() * g).real();
    return std::vector<double>(grad.data(), grad.data() + grad.size());
}

const Eigen::MatrixXcd& DRTForwardModel::getMatrix() const {
    return A_;
}
