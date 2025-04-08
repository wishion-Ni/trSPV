#pragma once
#include <string>
#include <memory>
#include "../../Plugins/lib/ComplexLossFunction.h"
#include "../../Plugins/lib/RegularizationTerm.h"

class ComplexLossFunction;
class RegularizationTerm;
class LossLoader;

class DynRecContext {
public:
    bool loadLossDLL(const std::string& path);
    bool loadRegDLL(const std::string& path);

    bool setLossFunctionFromJSON(const std::string& json);
    bool setRegularizationFromJSON(const std::string& json);

    std::shared_ptr<ComplexLossFunction> getLossFunction() const;
    std::shared_ptr<RegularizationTerm> getRegularization() const;
    void* getRawRegularizationHandle() const;

private:
    std::shared_ptr<ComplexLossFunction> loss_function_ = nullptr;
    std::shared_ptr<RegularizationTerm> regularization_ = nullptr;
};
