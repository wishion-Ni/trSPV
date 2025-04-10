#include "ResultExporter.h"
#include <fstream>
#include <iomanip>

bool ResultExporter::exportDRT(const std::string& filepath,
    const std::vector<double>& tau,
    const std::vector<double>& drt) {
    if (tau.size() != drt.size()) return false;
    std::ofstream fout(filepath);
    if (!fout) return false;

    fout << "tau\tDRT\n";
    for (size_t i = 0; i < tau.size(); ++i) {
        fout << std::scientific << std::setprecision(6)
            << tau[i] << "\t" << drt[i] << "\n";
    }
    return true;
}

bool ResultExporter::exportPrediction(const std::string& filepath,
    const std::vector<double>& periods,
    const std::vector<std::complex<double>>& predicted,
    const std::vector<std::complex<double>>& measured) {
    if (predicted.size() != measured.size() || measured.size() != periods.size()) return false;
    std::ofstream fout(filepath);
    if (!fout) return false;

    fout << "Period\tRe_measured\tIm_measured\tRe_predicted\tIm_predicted\n";
    for (size_t i = 0; i < predicted.size(); ++i) {
        fout << std::scientific << std::setprecision(6)
            << periods[i] << "\t"
            << std::real(measured[i]) << "\t"
            << std::imag(measured[i]) << "\t"
            << std::real(predicted[i]) << "\t"
            << std::imag(predicted[i]) << "\n";
    }
    return true;
}

bool ResultExporter::exportFitMetrics(const std::string& filepath, const FitMetrics& metrics) {
    std::ofstream fout(filepath);
    if (!fout) return false;

    fout << "RSS\tRMSE\tAmplitudeError\tPhaseErrorDeg\n";
    fout << std::scientific << std::setprecision(6)
        << metrics.rss << "\t"
        << metrics.rmse << "\t"
        << metrics.amplitude_error << "\t"
        << metrics.phase_error_deg << "\n";
    return true;
}