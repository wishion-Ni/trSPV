#pragma once

#include <vector>
#include <complex>
#include <string>
#include <nlohmann/json.hpp>

class TikhonovSolverWrapper {
public:
    ~TikhonovSolverWrapper();

    bool load(const std::string& dll_path);
    bool configureFromJSON(const nlohmann::json& config);
    bool setProblem(const std::vector<std::complex<double>>& A,
        const std::vector<std::complex<double>>& b,
        int m, int n);
    bool setLambdaRange(double lambda_min, double lambda_max, int num);
    bool findBestLambda(double& lambda_out);
    bool exportLcurve(const std::string& path);
    std::vector<double> getSolution(double lambda_out);


private:
    void* selector_ = nullptr;
    int m_ = 0, n_ = 0;
    std::vector<std::complex<double>> A_;
    std::vector<std::complex<double>> b_;
};
