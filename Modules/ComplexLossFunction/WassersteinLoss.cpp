#include "WassersteinLoss.h"
#include "ComplexL1LossFunction.h"

double WassersteinLoss::evaluate(const std::vector<std::complex<double>>& prediction,
    const std::vector<std::complex<double>>& target) const {
    double loss = 0.0;
    for (size_t i = 0; i < prediction.size(); ++i) {
        loss += std::abs(prediction[i] - target[i]);
    }
    return loss;
}

std::vector<std::complex<double>> WassersteinLoss::gradient(const std::vector<std::complex<double>>& prediction,
    const std::vector<std::complex<double>>& target) const {
    return ComplexL1LossFunction().gradient(prediction, target);
}
