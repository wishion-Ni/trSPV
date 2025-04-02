#include "RegTermRegistry.h"
#include <iostream>

RegTermRegistry& RegTermRegistry::instance() {
    static RegTermRegistry inst;
    return inst;
}

void RegTermRegistry::registerBuilder(const std::string& name, BuilderFunc func) {
    builders_[name] = std::move(func);
}

std::unique_ptr<RegularizationTerm> RegTermRegistry::create(const RegConfig& cfg, int depth) const {
    auto it = builders_.find(cfg.type);
    if (it != builders_.end()) {
        return it->second(cfg, depth);
    }
    std::cerr << "Unknown regularization type: " << cfg.type << std::endl;
    return nullptr;
}