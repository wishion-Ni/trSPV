#include "DynRecLogger.h"
#include <iomanip>
#include <ctime>

bool DynRecLogger::startSession(const std::string& filepath) {
    log_file_.open(filepath);
    is_open_ = log_file_.is_open();
    return is_open_;
}

void DynRecLogger::logHeader(const std::string& loss_name,
    const nlohmann::json& loss_cfg,
    const std::string& reg_name,
    const nlohmann::json& reg_cfg,
    double reg_weight,
    const std::vector<std::complex<double>>& target,
    const std::vector<double>& x0) {
    if (!is_open_) return;

    std::time_t now = std::time(nullptr);
    char time_buf[64] = {};
    std::tm local_tm{};
#if defined(_MSC_VER)
    localtime_s(&local_tm, &now);
#else
    local_tm = *std::localtime(&now);
#endif
    std::strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", &local_tm);

    log_file_ << "# DynRec Optimization Log\n";
    log_file_ << "# Timestamp: " << time_buf << "\n";

    log_file_ << "\n[Configuration]\n";
    log_file_ << "Loss Function: " << loss_name << "\n";
    log_file_ << "Loss Config: " << loss_cfg.dump() << "\n";
    log_file_ << "Regularization: " << reg_name << "\n";
    log_file_ << "Regularization Config: " << reg_cfg.dump() << "\n";
    log_file_ << "Lambda: " << reg_weight << "\n";

    log_file_ << "Target: \n";
    for (size_t i = 0; i < target.size(); ++i)
        log_file_ << target[i] << " " << "\n";
    log_file_ << "...\n";

    log_file_ << "Initial x0: \n";
    for (double v : x0) log_file_ << v << " " << "\n";
    log_file_ << "\n";

}

void DynRecLogger::logOptimization(const std::string& opt_name,
    const nlohmann::json& opt_cfg) {
    if (!is_open_) return;
    log_file_ << "\n[Optimization]\n";
    log_file_ << "Optimization Name: " << opt_name << "\n";
    log_file_ << "Optimization Config: " << opt_cfg.dump() << "\n";

    log_file_ << "\n[Iterations]\n";
    log_file_ << "Iter\tLoss\tGradientNorm\n";
}

void DynRecLogger::logIteration(int iter, double loss_val, double grad_norm) {
    if (!is_open_) return;
    log_file_ << iter << "\t"
        << std::fixed << std::setprecision(8) << loss_val << "\t"
        << grad_norm << "\n";
}

void DynRecLogger::logFinalResult(const std::vector<double>& x_opt,
    double final_loss,
    int total_iters,
    const std::string& stop_reason) {
    if (!is_open_) return;
    log_file_ << "\n[Result]\n";
    log_file_ << "Final Loss: " << final_loss << "\n";
    log_file_ << "Iterations: " << total_iters << "\n";
    log_file_ << "Stop Reason: " << stop_reason << "\n";

    log_file_ << "x_opt: ";
    for (double v : x_opt) log_file_ << v << " ";
    log_file_ << "\n";

    // Ð´Èë JSON summary
    nlohmann::json summary;
    summary["final_loss"] = final_loss;
    summary["iterations"] = total_iters;
    summary["stop_reason"] = stop_reason;
    summary["x_opt"] = x_opt;

    log_file_ << "\n[Summary(JSON)]\n";
    log_file_ << summary.dump(4) << "\n";
}


void DynRecLogger::endSession() {
    if (is_open_) {
        log_file_.close();
        is_open_ = false;
    }
}
