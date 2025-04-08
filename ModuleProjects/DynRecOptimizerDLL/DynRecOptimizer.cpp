#include "DynRecOptimizer.h"
#include "LossLoader.h"
#include "RegLoader.h"
#include "ForwardModel.h"
#include "OptimizerFactory.h"
#include <iostream>
#include "DynRecLogger.h"
#include <filesystem>
#include <chrono>

namespace fs = std::filesystem;

bool DynRecOptimizer::loadPlugins(const std::string& loss_dll_path, const std::string& reg_dll_path) {
    bool loss_ok = LossLoader::instance().load(loss_dll_path);
    bool reg_ok = RegLoader::instance().load(reg_dll_path);
    return loss_ok && reg_ok;
}

bool DynRecOptimizer::configure(const Eigen::MatrixXcd& A,
    const std::vector<std::complex<double>>& target,
    const std::string& loss_json,
    const std::string& reg_json,
    double reg_weight,
    const std::vector<double>& x0) {
    model_ = std::make_shared<EigenForwardModel>(A);
    target_ = target;
    reg_weight_ = reg_weight;
    x0_ = x0.empty() ? std::vector<double>(A.cols(), 0.0) : x0;

    loss_config_ = nlohmann::json::parse(loss_json);
    reg_config_ = nlohmann::json::parse(reg_json);

    loss_ = LossLoader::instance().createFromJSON(loss_json);
    if (!loss_) {
        std::cerr << "Failed to create loss from JSON." << std::endl;
        return false;
    }

    auto reg = RegLoader::instance().createFromJSON(reg_json);
    if (!reg) {
        std::cerr << "Failed to create regularization from JSON." << std::endl;
        return false;
    }
    reg_handle_ = RegLoader::instance().raw_handle();
    std::string reg_name = RegLoader::instance().name_func(reg_handle_);

    std::string log_path = "logs/dynrec_log_" + std::to_string(std::time(nullptr)) + ".txt";
    fs::create_directories("logs");
    logger_.startSession(log_path);
    logger_.logHeader(loss_->name(), loss_config_, reg_name, reg_config_, reg_weight_, target_, x0_);

    return true;
}

std::vector<double> DynRecOptimizer::optimize(const std::string& method, const nlohmann::json& optimizer_config) {
    OptimizationProblem problem;
    problem.model = model_;
    problem.loss = loss_;
    problem.reg_handle = reg_handle_;
    problem.reg_weight = reg_weight_;
    problem.target = target_;
    problem.x0 = x0_;

    auto opt = OptimizerFactory::create(method, optimizer_config);
    if (!opt) {
        std::cerr << "Unknown optimizer: " << method << std::endl;
        return {};
    }

    logger_.logOptimization(method, optimizer_config);

    opt->setLogger(&logger_);
    return opt->optimize(problem);
}
