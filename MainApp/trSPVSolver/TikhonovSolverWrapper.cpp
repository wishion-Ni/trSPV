#include "TikhonovSolverWrapper.h"
#include <windows.h>
#include <stdexcept>
#include <iostream>

static HMODULE tikh_dll = nullptr;
static void* (*CreateSelectorPtr)() = nullptr;
static void (*SetProblemPtr)(void*, const std::complex<double>*, int, int, const std::complex<double>*, const std::complex<double>*, int, int) = nullptr;
static void (*SetLambdaLogSpacePtr)(void*, double, double, int) = nullptr;
static void (*SetCriterionPtr)(void*, int) = nullptr;
static void (*SetLcurveCornerMethodPtr)(void*, int) = nullptr;
static double (*FindBestLambdaPtr)(void*) = nullptr;
static bool (*ExportLcurveCSVPtr)(void*, const char*) = nullptr;
static bool (*GetSolutionPtr)(double, void*, double*) = nullptr;

TikhonovSolverWrapper::~TikhonovSolverWrapper() {
    if (tikh_dll) {
        FreeLibrary(tikh_dll);
        tikh_dll = nullptr;
    }
    selector_ = nullptr;
}

bool TikhonovSolverWrapper::load(const std::string& dll_path) {
    tikh_dll = LoadLibraryA("plugins/TikhonovRegularizationDLL.dll");
    if (!tikh_dll) return false;

    CreateSelectorPtr = (decltype(CreateSelectorPtr))GetProcAddress(tikh_dll, "CreateSelector");
    SetProblemPtr = (decltype(SetProblemPtr))GetProcAddress(tikh_dll, "SetProblem");
    SetLambdaLogSpacePtr = (decltype(SetLambdaLogSpacePtr))GetProcAddress(tikh_dll, "SetLambdaLogSpace");
    SetCriterionPtr = (decltype(SetCriterionPtr))GetProcAddress(tikh_dll, "SetCriterion");
    SetLcurveCornerMethodPtr = (decltype(SetLcurveCornerMethodPtr))GetProcAddress(tikh_dll, "SetLcurveCornerMethod");
    FindBestLambdaPtr = (decltype(FindBestLambdaPtr))GetProcAddress(tikh_dll, "FindBestLambda");
    ExportLcurveCSVPtr = (decltype(ExportLcurveCSVPtr))GetProcAddress(tikh_dll, "ExportLcurveCSV");
    GetSolutionPtr = (decltype(GetSolutionPtr))GetProcAddress(tikh_dll, "GetSolution");

    if (!CreateSelectorPtr || !SetProblemPtr || !SetLambdaLogSpacePtr || !SetCriterionPtr ||
        !SetLcurveCornerMethodPtr || !FindBestLambdaPtr || !ExportLcurveCSVPtr || !GetSolutionPtr)
        return false;

    selector_ = CreateSelectorPtr();
    return selector_ != nullptr;
}


bool TikhonovSolverWrapper::setProblem(const std::vector<std::complex<double>>& A,
    const std::vector<std::complex<double>>& b,
    int m, int n) {
    A_ = A;
    b_ = b;
    m_ = m;
    n_ = n;
    SetProblemPtr(selector_, A_.data(), m_, n_, b_.data(), nullptr, 0, 0);
    return true;
}

bool TikhonovSolverWrapper::setLambdaRange(double lambda_min, double lambda_max, int num) {
    SetLambdaLogSpacePtr(selector_, lambda_min, lambda_max, num);
    return true;
}

bool TikhonovSolverWrapper::findBestLambda(double& lambda_out) {
    lambda_out = FindBestLambdaPtr(selector_);
    return true;
}

std::vector<double> TikhonovSolverWrapper::getSolution(double lambda) {
    std::vector<double> x(n_, 0.0);
    if (!GetSolutionPtr(lambda,selector_, x.data())) {
        throw std::runtime_error("[TikhonovSolverWrapper] GetSolution failed.");
    }
    return x;
}


bool TikhonovSolverWrapper::exportLcurve(const std::string& path) {
    return ExportLcurveCSVPtr(selector_, path.c_str());
}

bool TikhonovSolverWrapper::configureFromJSON(const nlohmann::json& config) {
    try {
        double lambda_min = config.value("lambda_min", 1e-6);
        double lambda_max = config.value("lambda_max", 1e2);
        int lambda_num = config.value("lambda_num", 50);
        std::string criterion = config.value("criterion", "lcurve");
        std::string method = config.value("corner_method", "max_curvature");
        std::string lcurve_path = config.value("export_lcurve", "");

        setLambdaRange(lambda_min, lambda_max, lambda_num);

        if (criterion == "lcurve") {
            SetCriterionPtr(selector_, 0);
        }
        else if (criterion == "validation") {
            SetCriterionPtr(selector_, 1);
        }
        else if (criterion == "gcv") {
            SetCriterionPtr(selector_, 2);
        }
        else {
            std::cerr << "[TikhonovSolverWrapper] Unsupported criterion: " << criterion << std::endl;
            return false;
        }

        if (method == "max_curvature") {
            SetLcurveCornerMethodPtr(selector_, 0);
        }
        else if (method == "triangle") {
            SetLcurveCornerMethodPtr(selector_, 1);
        }
        else {
            std::cerr << "[TikhonovSolverWrapper] Unsupported corner method: " << method << std::endl;
            return false;
        }

        if (!lcurve_path.empty()) {
            exportLcurve(lcurve_path);
        }
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "[TikhonovSolverWrapper] Failed to parse Tikhonov config: " << e.what() << std::endl;
        return false;
    }
}