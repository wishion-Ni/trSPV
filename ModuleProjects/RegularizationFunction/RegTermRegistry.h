#pragma once

#include "RegularizationTerm.h"
#include "RegConfig.h"
#include <map>
#include <functional>
#include <memory>

class RegTermRegistry {
public:
    using BuilderFunc = std::function<std::unique_ptr<RegularizationTerm>(const RegConfig&, int)>;

    static RegTermRegistry& instance();

    void registerBuilder(const std::string& name, BuilderFunc func);
    std::unique_ptr<RegularizationTerm> create(const RegConfig& cfg, int depth = 0) const;

private:
    std::map<std::string, BuilderFunc> builders_;
};
