#include "TikhonovParameterSelector.h"
#include <iostream>
#include <complex>
#include <vector>
#include <cmath>

int main() {
    // 构造一个简单测试问题 A*x = b
    const int m = 5, n = 3;
    std::vector<std::complex<double>> A = {
        {1,0}, {2,0}, {3,0},
        {4,0}, {5,0}, {6,0},
        {7,0}, {8,0}, {9,0},
        {1,0}, {0,0}, {1,0},
        {0,0}, {1,0}, {0,0},
    };
    std::vector<std::complex<double>> x_true = { {1,0}, {0.5,0}, {-0.5,0} };
    std::vector<std::complex<double>> b(m);
    for (int i = 0; i < m; ++i) {
        b[i] = 0.0;
        for (int j = 0; j < n; ++j) {
            b[i] += A[i * n + j] * x_true[j];
        }
    }

    // 正则化矩阵 L = I
    std::vector<std::complex<double>> L = {
        {1,0}, {0,0}, {0,0},
        {0,0}, {1,0}, {0,0},
        {0,0}, {0,0}, {1,0},
    };

    TikhonovParameterSelector selector;
    selector.setProblem(A, m, n, b, L, n, n);
    selector.setLogSpace(1e-6, 1e2, 50);

    // === Test 1: L-curve 最大曲率法 ===
    selector.setCriterion(TikhonovParameterSelector::CriterionType::Lcurve);
    selector.setLcurveCornerMethod(TikhonovParameterSelector::LcurveCornerMethod::MaxCurvature);
    double lambda1 = selector.findBestLambda();
    std::cout << "[L-curve MaxCurvature] Best lambda = " << lambda1 << std::endl;
    selector.exportLcurveToCSV("lcurve_max_curvature.csv");

    // === Test 2: L-curve 三角法 ===
    selector.setLcurveCornerMethod(TikhonovParameterSelector::LcurveCornerMethod::TriangleMethod);
    double lambda2 = selector.findBestLambda();
    std::cout << "[L-curve TriangleMethod] Best lambda = " << lambda2 << std::endl;
    selector.exportLcurveToCSV("lcurve_triangle_method.csv");

    // === Test 3: 验证集最小误差 ===
    selector.setCriterion(TikhonovParameterSelector::CriterionType::Validation);
    selector.setValidationSet(A, b, m);  // 用原始数据模拟验证集（仅测试）
    double lambda3 = selector.findBestLambda();
    std::cout << "[Validation Error] Best lambda = " << lambda3 << std::endl;

    // === Test 4: GCV ===
    selector.setCriterion(TikhonovParameterSelector::CriterionType::GCV);
    double lambda4 = selector.findBestLambda();
    std::cout << "[GCV] Best lambda = " << lambda4 << std::endl;

    return 0;
}
