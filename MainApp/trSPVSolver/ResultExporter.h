#pragma once

#include <string>
#include <vector>
#include <complex>
#include "FitEvaluator.h"

class ResultExporter {
public:
    // ���� tau vs DR(tau)
    static bool exportDRT(const std::string& filepath,
        const std::vector<double>& tau,
        const std::vector<double>& drt);

    // ����Ԥ�� vs ʵ�⣨�������������ѡ��
    static bool exportPrediction(const std::string& filepath,
        const std::vector<double>& periods,
        const std::vector<std::complex<double>>& predicted,
        const std::vector<std::complex<double>>& measured);

    static bool exportFitMetrics(const std::string& filepath, const FitMetrics& metrics);
};