#include "DynRecContext.h"
#include "RegLoader.h"
#include "LossLoader.h"
#include "AdamOptimizer.h"
#include "ConjugateGradientOptimizer.h"
#include "PSOOptimizer.h"
#include "DEOptimizer.h"
#include "ForwardModel.h"
#include <Eigen/Dense>
#include <iostream>
#include <chrono>

void test_optimizer(const std::string& name, std::unique_ptr<Optimizer> optimizer, const OptimizationProblem& problem) {
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<double> x_opt = optimizer->optimize(problem);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "\n==== Result for " << name << " ====" << std::endl;
    for (double x : x_opt) std::cout << x << " ";
    std::cout << "\nElapsed time: " << elapsed.count() << "s\n";
}

int main() {
    // DLL 加载
    if (!LossLoader::instance().load("../../Plugins/ComplexLossFunctionDLL.dll") ||
        !RegLoader::instance().load("../../Plugins/RegularizationFunction.dll")) {
        std::cerr << "Failed to load DLLs" << std::endl;
        return 1;
    }

    // 构造模型数据
    Eigen::MatrixXcd A(3, 3);
    A << std::complex<double>(1.0, 0.0), std::complex<double>(0.5, 0.1), std::complex<double>(0.2, -0.3),
        std::complex<double>(0.1, 0.2), std::complex<double>(1.0, 0.0), std::complex<double>(0.3, 0.1),
        std::complex<double>(0.0, -0.1), std::complex<double>(0.4, 0.3), std::complex<double>(1.0, 0.0);

    std::vector<std::complex<double>> target = { {1.2, 0.3}, {2.1, 0.4}, {1.8, -0.2} };
    std::vector<double> x0 = { 0.0, 0.0, 0.0 };

    auto model = std::make_shared<EigenForwardModel>(A);
    std::string loss_json = R"({"type":"AmplitudePhaseL2", "numeric":{"amp_weight":1.0,"phase_weight":1.0}})";
    std::string reg_json = R"({"type":"L2"})";

    auto loss = LossLoader::instance().createFromJSON(loss_json);
    auto reg = RegLoader::instance().createFromJSON(reg_json);

    OptimizationProblem problem;
    problem.model = model;
    problem.loss = loss;
    problem.reg_handle = RegLoader::instance().raw_handle();
    problem.reg_weight = 0.01;
    problem.target = target;
    problem.x0 = x0;

    // 构造并测试优化器
    test_optimizer("Adam", std::make_unique<AdamOptimizer>(nlohmann::json{ {"learning_rate", 0.05} }), problem);
    test_optimizer("CG", std::make_unique<ConjugateGradientOptimizer>(nlohmann::json{}), problem);
    test_optimizer("PSO", std::make_unique<PSOOptimizer>(nlohmann::json{ {"swarm_size", 30}, {"max_iter", 100}, {"refine", true} }), problem);
    test_optimizer("DE", std::make_unique<DEOptimizer>(nlohmann::json{ {"pop_size", 30}, {"max_iter", 100}, {"refine", true} }), problem);

    return 0;
}