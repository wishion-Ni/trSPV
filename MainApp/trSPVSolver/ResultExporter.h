#pragma once

#include <string>
#include <vector>
#include <complex>
#include "FitEvaluator.h"

class ResultExporter {
public:
    // 导出 tau vs DR(tau)
    static bool exportDRT(const std::string& filepath,
        const std::vector<double>& tau,
        const std::vector<double>& drt);

    // 导出预测 vs 实测（拟合质量评估可选）
    static bool exportPrediction(const std::string& filepath,
        const std::vector<double>& periods,
        const std::vector<std::complex<double>>& predicted,
        const std::vector<std::complex<double>>& measured);

    static bool exportFitMetrics(const std::string& filepath, const FitMetrics& metrics);
};