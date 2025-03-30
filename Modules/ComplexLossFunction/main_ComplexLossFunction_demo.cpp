// test_loss_modules.cpp
#include <iostream>
#include <vector>
#include <complex>
#include <memory>

#include "ComplexL2LossFunction.h"
#include "ComplexL1LossFunction.h"
#include "ComplexPseudoHuberLoss.h"
#include "ComplexLogCoshLoss.h"
#include "AmplitudePhaseL2Loss.h"
#include "ModulusSquaredDiffLoss.h"
#include "CosineSimilarityLoss.h"
#include "KLDivLoss.h"
#include "JSDivLoss.h"
#include "WassersteinLoss.h"
#include "ComplexIoULoss.h"
#include "CompositeLossFunction.h"

void printComplexVector(const std::vector<std::complex<double>>& vec, const std::string& label) {
    std::cout << label << ": [ ";
    for (const auto& z : vec) {
        std::cout << z << " ";
    }
    std::cout << "]\n";
}

int main() {
    std::vector<std::complex<double>> pred = {
        {1.0, 2.0}, {0.5, -0.3}, {0.2, 0.1}
    };
    std::vector<std::complex<double>> target = {
        {1.5, 1.8}, {0.4, -0.1}, {0.0, 0.0}
    };

    std::vector<std::shared_ptr<ComplexLossFunction>> losses = {
        std::make_shared<ComplexL2LossFunction>(),
        std::make_shared<ComplexL1LossFunction>(),
        std::make_shared<ComplexPseudoHuberLoss>(1.0),
        std::make_shared<ComplexLogCoshLoss>(),
        std::make_shared<AmplitudePhaseL2Loss>(1.0, 0.5),
        std::make_shared<ModulusSquaredDiffLoss>(),
        std::make_shared<CosineSimilarityLoss>(),
        std::make_shared<KLDivLoss>(),
        std::make_shared<JSDivLoss>(),
        std::make_shared<WassersteinLoss>(),
        std::make_shared<ComplexIoULoss>()
    };

    for (const auto& loss : losses) {
        std::cout << "Testing: " << loss->name() << std::endl;
        double val = loss->evaluate(pred, target);
        auto grad = loss->gradient(pred, target);
        std::cout << "Loss value: " << val << std::endl;
        printComplexVector(grad, "Gradient");
        std::cout << "-----------------------------\n";
    }

    // ²âÊÔ×éºÏËðÊ§
    CompositeLossFunction composite;
    composite.addLoss(std::make_shared<ComplexL2LossFunction>(), 1.0);
    composite.addLoss(std::make_shared<KLDivLoss>(), 0.5);

    std::cout << "Testing: CompositeLossFunction" << std::endl;
    double compVal = composite.evaluate(pred, target);
    auto compGrad = composite.gradient(pred, target);
    std::cout << "Composite Loss value: " << compVal << std::endl;
    printComplexVector(compGrad, "Composite Gradient");

    return 0;
}
