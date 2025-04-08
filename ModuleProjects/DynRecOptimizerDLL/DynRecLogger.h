#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <complex>
#include <nlohmann/json.hpp>

class DynRecLogger {
public:
    bool startSession(const std::string& filepath);

    void logHeader(const std::string& loss_name,
        const nlohmann::json& loss_cfg,
        const std::string& reg_name,
        const nlohmann::json& reg_cfg,
        double reg_weight,
        const std::vector<std::complex<double>>& target,
        const std::vector<double>& x0);

    void logOptimization(const std::string& opt_name,
        const nlohmann::json& opt_cfg);

    void logIteration(int iter, double loss_val, double grad_norm);

    void logFinalResult(const std::vector<double>& x_opt,
        double final_loss,
        int total_iters,
        const std::string& stop_reason);

    void endSession();

    inline bool isActive() const { return is_open_; }

private:
    std::ofstream log_file_;
    bool is_open_ = false;
};

