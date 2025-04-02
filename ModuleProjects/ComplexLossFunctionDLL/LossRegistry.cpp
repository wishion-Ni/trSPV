#include <mutex>
#include "LossRegistry.h"
#include "LossBuiltinRegistration.h"

LossRegistry& LossRegistry::instance() {
    static LossRegistry inst;
    return inst;
}


void InitLossFactory() {
    static std::once_flag flag;
    std::call_once(flag, []() {
        RegisterAllLossFunctions(LossRegistry::instance());
        });
}

void LossRegistry::registerBuilder(const std::string& name, LossBuilder builder) {
    builders_[name] = std::move(builder);
}

std::unique_ptr<ComplexLossFunction> LossRegistry::create(const LossConfig& config, int depth) {
    auto it = builders_.find(config.type);
    if (it != builders_.end()) {
        return it->second(config, depth);
    }
    return nullptr;
}