#include <string>
#include <memory>
#include <unordered_map>
#include <iostream>
#include <stdexcept>
#include "ComplexLossFunctionFactor.h"

#include "CompositeLossFunction.h"
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

#include "LossRegistry.h"
#include "LossConfig.h"

void InitLossFactory();

ComplexLossFunction* CreateComplexLossFunction(const char* name) {
    std::string n(name);
    if (n == "L2") return new ComplexL2LossFunction();
    if (n == "L1") return new ComplexL1LossFunction();
    if (n == "Huber") return new ComplexPseudoHuberLoss(1.0);
    if (n == "LogCosh") return new ComplexLogCoshLoss();

    if (n == "AmplitudePhaseL2") return new AmplitudePhaseL2Loss();
    if (n == "ModulusSquaredDiff") return new ModulusSquaredDiffLoss();
    if (n == "CosineSimilarity") return new CosineSimilarityLoss();
    if (n == "ComplexIoU") return new ComplexIoULoss();

    if (n == "KLDiv") return new KLDivLoss();
    if (n == "JSDiv") return new JSDivLoss();
    if (n == "Wasserstein") return new WassersteinLoss();

    if (n == "Composite") return new CompositeLossFunction();

    return nullptr;
}

void DestroyComplexLossFunction(ComplexLossFunction* ptr) {
    delete ptr;
}

ComplexLossFunction* CreateComplexLossFunctionFromJSON(const char* json_str) {
    InitLossFactory();
    LossConfig config;
    if (!ParseLossConfigFromJSON(json_str, config)) {
        std::cerr << "Failed to parse JSON loss config." << std::endl;
        return nullptr;
    }
    std::unique_ptr<ComplexLossFunction> ptr = LossRegistry::instance().create(config);
    return ptr ? ptr.release() : nullptr;
}