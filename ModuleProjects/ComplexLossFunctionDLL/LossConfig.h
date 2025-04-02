#pragma once
#include <string>
#include <unordered_map>

struct LossConfig {
    std::string type;
    std::unordered_map<std::string, double> numeric;
    std::unordered_map<std::string, std::string> symbolic;
    std::vector<std::pair<LossConfig, double>> composite_list;
};

bool ParseLossConfigFromJSON(const std::string& json_str, LossConfig& config);