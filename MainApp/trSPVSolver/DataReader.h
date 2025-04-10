#pragma once

#include <string>
#include <vector>
#include <complex>

class DataReader {
public:

    static bool readCSV(const std::string& filename,
        std::vector<double>& freq_or_period,
        std::vector<std::complex<double>>& spv_data);
};
