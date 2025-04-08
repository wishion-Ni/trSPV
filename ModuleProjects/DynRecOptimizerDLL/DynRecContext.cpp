#include "DynRecContext.h"
#include "LossLoader.h"
#include "RegLoader.h"
#include <iostream>

bool DynRecContext::loadLossDLL(const std::string& path) {
    return LossLoader::instance().load(path);
}

bool DynRecContext::loadRegDLL(const std::string& path) {
    return RegLoader::instance().load(path);
}

bool DynRecContext::setLossFunctionFromJSON(const std::string& json) {
    auto ptr = LossLoader::instance().createFromJSON(json);
    if (!ptr) {
        std::cerr << "[DynRecContext] Failed to create loss function." << std::endl;
        return false;
    }
    loss_function_ = std::move(ptr);
    return true;
}

bool DynRecContext::setRegularizationFromJSON(const std::string& json) {
    auto ptr = RegLoader::instance().createFromJSON(json);
    if (!ptr) {
        std::cerr << "[DynRecContext] Failed to create regularization." << std::endl;
        return false;
    }
    regularization_ = std::move(ptr);
    return true;
}

std::shared_ptr<ComplexLossFunction> DynRecContext::getLossFunction() const {
    return loss_function_;
}

std::shared_ptr<RegularizationTerm> DynRecContext::getRegularization() const {
    return regularization_;
}

void* DynRecContext::getRawRegularizationHandle() const {
    return RegLoader::instance().raw_handle();
}