#include "RegTermRegistry.h"
#include "L2Regularization.h"
#include "L1Regularization.h"
#include "TVRegularization.h"
#include "HuberRegularization.h"
#include "LaplacianRegularization.h"
#include "CompositeRegularization.h"
#include <iostream>

#include "RegisterAllRegularizationTerms.h"

void RegisterAllRegularizationTerms(RegTermRegistry& reg) {
    reg.registerBuilder("L2", [](const RegConfig&, int) {
        return std::make_unique<L2Regularization>();
        });
    reg.registerBuilder("L1", [](const RegConfig&, int) {
        return std::make_unique<L1Regularization>();
        });
    reg.registerBuilder("TV", [](const RegConfig&, int) {
        return std::make_unique<TVRegularization>();
        });
    reg.registerBuilder("Huber", [](const RegConfig& cfg, int) {
        double delta = cfg.numeric.count("delta") ? cfg.numeric.at("delta") : 1.0;
        return std::make_unique<HuberRegularization>(delta);
        });
    reg.registerBuilder("Laplacian", [](const RegConfig&, int) {
        return std::make_unique<LaplacianRegularization>();
        });
    reg.registerBuilder("Composite", [](const RegConfig& cfg, int depth) {
        if (depth > 10) {
            std::cerr << "Composite nesting too deep. Aborting." << std::endl;
            return std::unique_ptr<RegularizationTerm>{};
        }
        auto composite = std::make_unique<CompositeRegularization>();
        for (const auto& [subcfg, weight] : cfg.composite_list) {
            auto subterm = RegTermRegistry::instance().create(subcfg, depth + 1);
            if (subterm) {
                composite->addTerm(std::move(subterm), weight);
            }
        }
        return std::unique_ptr<RegularizationTerm>(std::move(composite));
        });
}
