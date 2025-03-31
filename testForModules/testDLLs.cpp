#include <windows.h>
#include <iostream>
#include <vector>
#include <complex>

typedef void* (__cdecl* CreateSelector_t)();
typedef void(__cdecl* DestroySelector_t)(void*);
typedef void(__cdecl* SetProblem_t)(void*, const std::complex<double>*, int, int, const std::complex<double>*, const std::complex<double>*, int, int);
typedef void(__cdecl* SetLambdaLogSpace_t)(void*, double, double, int);
typedef void(__cdecl* SetCriterion_t)(void*, int);
typedef void(__cdecl* SetLcurveCornerMethod_t)(void*, int);
typedef void(__cdecl* SetValidationSet_t)(void*, const std::complex<double>*, const std::complex<double>*, int);
typedef double(__cdecl* FindBestLambda_t)(void*);
typedef bool(__cdecl* ExportLcurveCSV_t)(void*, const char*);

int main() {
    HMODULE hDll = LoadLibraryA("../../Plugins/TikhonovRegularizationDLL.dll");
    if (!hDll) {
        std::cerr << "Failed to load DLL!" << std::endl;
        return -1;
    }

    auto CreateSelector = (CreateSelector_t)GetProcAddress(hDll, "CreateSelector");
    auto DestroySelector = (DestroySelector_t)GetProcAddress(hDll, "DestroySelector");
    auto SetProblem = (SetProblem_t)GetProcAddress(hDll, "SetProblem");
    auto SetLambdaLogSpace = (SetLambdaLogSpace_t)GetProcAddress(hDll, "SetLambdaLogSpace");
    auto SetCriterion = (SetCriterion_t)GetProcAddress(hDll, "SetCriterion");
    auto SetLcurveCornerMethod = (SetLcurveCornerMethod_t)GetProcAddress(hDll, "SetLcurveCornerMethod");
    auto SetValidationSet = (SetValidationSet_t)GetProcAddress(hDll, "SetValidationSet");
    auto FindBestLambda = (FindBestLambda_t)GetProcAddress(hDll, "FindBestLambda");
    auto ExportLcurveCSV = (ExportLcurveCSV_t)GetProcAddress(hDll, "ExportLcurveCSV");

    if (!CreateSelector || !DestroySelector || !SetProblem || !SetLambdaLogSpace ||
        !SetCriterion || !SetLcurveCornerMethod || !SetValidationSet || !FindBestLambda || !ExportLcurveCSV) {
        std::cerr << "Failed to bind one or more DLL functions!" << std::endl;
        return -2;
    }

    void* selector = CreateSelector();

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
    std::vector<std::complex<double>> L = {
        {1,0}, {0,0}, {0,0},
        {0,0}, {1,0}, {0,0},
        {0,0}, {0,0}, {1,0},
    };

    SetProblem(selector, A.data(), m, n, b.data(), L.data(), n, n);
    SetLambdaLogSpace(selector, 1e-6, 1e2, 50);

    SetCriterion(selector, 0); // L-curve
    SetLcurveCornerMethod(selector, 0); // MaxCurvature
    double lambda1 = FindBestLambda(selector);
    std::cout << "[DLL Dynamic] L-curve MaxCurvature best lambda = " << lambda1 << std::endl;
    ExportLcurveCSV(selector, "lcurve_dll_dynamic.csv");

    SetCriterion(selector, 1); // Validation
    SetValidationSet(selector, A.data(), b.data(), m);
    double lambda2 = FindBestLambda(selector);
    std::cout << "[DLL Dynamic] Validation best lambda = " << lambda2 << std::endl;

    SetCriterion(selector, 2); // GCV
    double lambda3 = FindBestLambda(selector);
    std::cout << "[DLL Dynamic] GCV best lambda = " << lambda3 << std::endl;

    DestroySelector(selector);
    FreeLibrary(hDll);
    return 0;
}