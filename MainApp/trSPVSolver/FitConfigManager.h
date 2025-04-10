#pragma once

#include <string>
#include <vector>
#include <complex>
#include <nlohmann/json.hpp>

class FitConfigManager {
public:
    bool loadFromFile(const std::string& filename);
    void applyCommandLineOverrides(int argc, char* argv[]);

    std::string getLossConfigJSON() const;
    std::string getRegConfigJSON() const;
    std::string getOptimizerConfigJSON() const;
    std::string getTikhonovConfigJSON() const;
    nlohmann::json getTikhonovConfigObject() const; 


    std::string getDataFilePath() const;
    std::string getLogFilePath() const;
    std::string getResultPath() const;
    std::string getEvaluationPath() const;
    std::string getLcurvePath() const;
    std::string getCustomX0Path() const;


    std::string getTauType() const;
    double getTauMin() const;
    double getTauMax() const;
    int getTauNumPoints() const;

    double getLambda() const;
    std::string getInitialGuessMode() const;

    void exportUsedConfig(const std::string& out_path) const;

private:
    nlohmann::json config_json_;
    void setDefaults();
    void applyOverride(const std::string& key, const std::string& value);
};
