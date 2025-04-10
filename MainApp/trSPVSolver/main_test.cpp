// main.cpp
#include "FitConfigManager.h"
#include "DataReader.h"
#include "TauGridGenerator.h"
#include "DRTForwardModel.h"
#include "InitialGuessGenerator.h"
#include "TikhonovInitialGuess.h"
#include "DynRecOptimizerRun.h"
#include "ResultExporter.h"
#include "FitEvaluator.h"
#include "Logger.h"

#include <iostream>

int main(int argc, char* argv[]) {
    try {
        // === 1. 配置初始化 ===
        FitConfigManager config;
        config.loadFromFile("config.json");
        Logger::init(config.getLogFilePath());

        // === 2. 读取实验数据 ===
        std::vector<double> periods;
        std::vector<std::complex<double>> spv;
        if (!DataReader::readCSV(config.getDataFilePath(), periods, spv)) {
            Logger::error("Failed to read data file");
            return 1;
        }

        // === 3. 构建 tau 网格 ===
        auto tau = TauGridGenerator::generateAuto(config.getTauType(), periods, config.getTauNumPoints());

        // === 4. 构建模型 A ===
        auto model = std::make_shared<DRTForwardModel>(tau, periods);
        const auto& A = model->getMatrix();

        int m = static_cast<int>(periods.size());
        int n = static_cast<int>(tau.size());
        std::vector<std::complex<double>> A_flat(m * n);
        for (int i = 0; i < m; ++i)
            for (int j = 0; j < n; ++j)
                A_flat[i * n + j] = A(i, j);

        // === 5. 生成初值 ===
        std::vector<double> x0;
        double lambda = config.getLambda();
        if (config.getInitialGuessMode() == "tikhonov") {
            x0 = generateTikhonovInitialGuess(A_flat, spv, m, n, config.getTikhonovConfigObject(), lambda);
        }
        else {
            x0 = InitialGuessGenerator::generate(config.getInitialGuessMode(), n, 1e-4, {}, config.getCustomX0Path());
        }

        // === 6. 构造优化问题结构 ===
        OptimizationProblem problem;
        problem.A_flat = A_flat;
        problem.target = spv;
        problem.x0 = x0;
        problem.m = m;
        problem.n = n;
        problem.loss_json = config.getLossConfigJSON();
        problem.reg_json = config.getRegConfigJSON();
        problem.optimizer_json = config.getOptimizerConfigJSON();
        problem.reg_weight = lambda;

        // === 7. 运行优化器 ===
        auto result = runOptimization(problem, problem.optimizer_json);

        // === 8. 输出结果 ===
        ResultExporter::exportDRT(config.getResultPath(), tau, result);

        auto predicted = model->apply(result);
        ResultExporter::exportPrediction("output/prediction.tsv", periods, predicted, spv);

        auto metrics = FitEvaluator::evaluate(predicted, spv);
        ResultExporter::exportFitMetrics(config.getEvaluationPath(), metrics);

        Logger::info("Optimization complete. RMSE = " + std::to_string(metrics.rmse));
        Logger::shutdown();

        return 0;

    }
    catch (const std::exception& e) {
        std::cerr << "[Fatal] " << e.what() << std::endl;
        Logger::error(e.what());
        Logger::shutdown();
        return -1;
    }
}
