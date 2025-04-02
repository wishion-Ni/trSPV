#include "LossBuiltinRegistration.h"
#include "LossRegistry.h"

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

#include <iostream>

void RegisterAllLossFunctions(LossRegistry& reg) {
    reg.registerBuilder("L2", [](const LossConfig&, int) {
        return std::make_unique<ComplexL2LossFunction>();
        });
    reg.registerBuilder("L1", [](const LossConfig&, int) {
        return std::make_unique<ComplexL1LossFunction>();
        });
    reg.registerBuilder("Huber", [](const LossConfig& cfg, int) {
        double delta = cfg.numeric.count("delta") ? cfg.numeric.at("delta") : 1.0;
        return std::make_unique<ComplexPseudoHuberLoss>(delta);
        });
    reg.registerBuilder("LogCosh", [](const LossConfig&, int) {
        return std::make_unique<ComplexLogCoshLoss>();
        });
    reg.registerBuilder("AmplitudePhaseL2", [](const LossConfig& cfg, int) {
        double w_amp = cfg.numeric.count("amp_weight") ? cfg.numeric.at("amp_weight") : 1.0;
        double w_phase = cfg.numeric.count("phase_weight") ? cfg.numeric.at("phase_weight") : 1.0;
        return std::make_unique<AmplitudePhaseL2Loss>(w_amp, w_phase);
        });
    reg.registerBuilder("ModulusSquaredDiff", [](const LossConfig&, int) {
        return std::make_unique<ModulusSquaredDiffLoss>();
        });
    reg.registerBuilder("CosineSimilarity", [](const LossConfig&, int) {
        return std::make_unique<CosineSimilarityLoss>();
        });
    reg.registerBuilder("KLDiv", [](const LossConfig&, int) {
        return std::make_unique<KLDivLoss>();
        });
    reg.registerBuilder("JSDiv", [](const LossConfig&, int) {
        return std::make_unique<JSDivLoss>();
        });
    reg.registerBuilder("Wasserstein", [](const LossConfig&, int) {
        return std::make_unique<WassersteinLoss>();
        });
    reg.registerBuilder("ComplexIoU", [](const LossConfig&, int) {
        return std::make_unique<ComplexIoULoss>();
        });
    reg.registerBuilder("Composite", [](const LossConfig& cfg, int depth) {
        if (depth > 10) {
            std::cerr << "Composite nesting too deep. Aborting." << std::endl;
            return std::unique_ptr<ComplexLossFunction>{};
        }
        auto composite = std::make_unique<CompositeLossFunction>();
        for (const auto& [subcfg, weight] : cfg.composite_list) {
            auto subloss = LossRegistry::instance().create(subcfg, depth + 1);
            if (subloss) {
                composite->addLoss(std::move(subloss), weight);
            }
        }
        return std::unique_ptr<ComplexLossFunction>(std::move(composite));
        });
}