// FitConfigManager.cpp
#include "FitConfigManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

using json = nlohmann::json;

bool FitConfigManager::loadFromFile(const std::string& filename) {
    std::ifstream fin(filename);
    if (!fin) throw std::runtime_error("[FitConfigManager] Failed to open config file: " + filename);
    fin >> config_json_;
    setDefaults();
    return true;
}

void FitConfigManager::applyCommandLineOverrides(int argc, char* argv[]) {
    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        auto pos = arg.find('=');
        if (pos == std::string::npos) continue;
        std::string key = arg.substr(0, pos);
        std::string value = arg.substr(pos + 1);
        applyOverride(key, value);
    }
}

void FitConfigManager::applyOverride(const std::string& key, const std::string& value) {
    config_json_[key] = value;
}

void FitConfigManager::setDefaults() {
    if (!config_json_.contains("loss")) config_json_["loss"] = { {"type", "L2"} };
    if (!config_json_.contains("regularization")) config_json_["regularization"] = { {"type", "L2"}, {"lambda", 0.01} };
    if (!config_json_.contains("optimizer")) config_json_["optimizer"] = { {"type", "adam"}, {"lr", 0.01}, {"max_iters", 500} };
    if (!config_json_.contains("initial_guess")) config_json_["initial_guess"] = "zero";
    if (!config_json_.contains("tau")) {
        config_json_["tau"] = {
            {"type", "log"},
            {"min", 1e-6},
            {"max", 1.0},
            {"num_points", 100}
        };
    }
}

std::string FitConfigManager::getLossConfigJSON() const {
    return config_json_["loss"].dump();
}

std::string FitConfigManager::getRegConfigJSON() const {
    return config_json_["regularization"].dump();
}

std::string FitConfigManager::getOptimizerConfigJSON() const {
    return config_json_["optimizer"].dump();
}

std::string FitConfigManager::getTikhonovConfigJSON() const {
    return config_json_.contains("tikhonov") ? config_json_["tikhonov"].dump() : "{}";
}

nlohmann::json FitConfigManager::getTikhonovConfigObject() const {
    return config_json_.contains("tikhonov") ? config_json_["tikhonov"] : nlohmann::json::object();
}


std::string FitConfigManager::getDataFilePath() const {
    return config_json_.value("data_file", "input.csv");
}

std::string FitConfigManager::getLogFilePath() const {
    return config_json_.value("log_file", "log/log.txt");
}

std::string FitConfigManager::getResultPath() const {
    return config_json_.value("result_file", "output/result.csv");
}

std::string FitConfigManager::getEvaluationPath() const {
    return config_json_.value("evaluation_file", "output/eval.csv");
}

std::string FitConfigManager::getLcurvePath() const {
    return config_json_.value("lcurve_file", "output/lcurve.csv");
}

std::string FitConfigManager::getCustomX0Path() const {
    return config_json_.value("custom_x0_file", "custom_x0.txt");
}


std::string FitConfigManager::getTauType() const {
    return config_json_["tau"].value("type", "log");
}

double FitConfigManager::getTauMin() const {
    return config_json_["tau"].value("min", 1e-6);
}

double FitConfigManager::getTauMax() const {
    return config_json_["tau"].value("max", 1.0);
}

int FitConfigManager::getTauNumPoints() const {
    return config_json_["tau"].value("num_points", 100);
}

double FitConfigManager::getLambda() const {
    return config_json_["regularization"].value("lambda", 0.01);
}

std::string FitConfigManager::getInitialGuessMode() const {
    return config_json_.value("initial_guess", "zero");
}

void FitConfigManager::exportUsedConfig(const std::string& out_path) const {
    std::ofstream fout(out_path);
    fout << config_json_.dump(4);
}
