#include "RegConfig.h"
#include "ParseRegConfigFromJSON.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

bool ParseRegConfigFromJSON(const std::string& json_str, RegConfig& config) {
    try {
        json j = json::parse(json_str);
        config.type = j.at("type").get<std::string>();

        if (j.contains("numeric")) {
            for (auto& [key, val] : j["numeric"].items()) {
                config.numeric[key] = val.get<double>();
            }
        }
        if (j.contains("symbolic")) {
            for (auto& [key, val] : j["symbolic"].items()) {
                config.symbolic[key] = val.get<std::string>();
            }
        }
        if (config.type == "Composite" && j.contains("composite")) {
            for (auto& item : j["composite"]) {
                RegConfig subcfg;
                subcfg.type = item.at("type").get<std::string>();

                if (item.contains("numeric")) {
                    for (auto& [k, v] : item["numeric"].items()) {
                        subcfg.numeric[k] = v.get<double>();
                    }
                }
                if (item.contains("symbolic")) {
                    for (auto& [k, v] : item["symbolic"].items()) {
                        subcfg.symbolic[k] = v.get<std::string>();
                    }
                }
                double weight = item.contains("weight") ? item["weight"].get<double>() : 1.0;
                config.composite_list.emplace_back(std::move(subcfg), weight);
            }
        }
        return true;
    }
    catch (...) {
        return false;
    }
}