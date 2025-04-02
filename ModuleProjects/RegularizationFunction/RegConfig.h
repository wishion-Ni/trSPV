#pragma once

#include <string>
#include <map>
#include <vector>
#include <utility>

struct RegConfig {
    std::string type;
    std::map<std::string, double> numeric;
    std::map<std::string, std::string> symbolic;
    std::vector<std::pair<RegConfig, double>> composite_list;
};