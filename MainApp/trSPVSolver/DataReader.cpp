#include "DataReader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <regex>

bool DataReader::readCSV(const std::string& filename,
    std::vector<double>& freq_or_period,
    std::vector<std::complex<double>>& spv_data) {
    std::ifstream fin(filename);
    if (!fin) {
        std::cerr << "[DataReader] Failed to open file: " << filename << std::endl;
        return false;
    }

    std::string line;
    std::regex delimiter("[\t,\x20]+"); 
    while (std::getline(fin, line)) {
        std::sregex_token_iterator it(line.begin(), line.end(), delimiter, -1);
        std::sregex_token_iterator end;
        std::vector<std::string> tokens(it, end);

        if (tokens.size() < 3) continue;

        try {
            double f = std::stod(tokens[0]);
            double re = std::stod(tokens[1]);
            double im = std::stod(tokens[2]);
            freq_or_period.push_back(f);
            spv_data.emplace_back(re, im);
        }
        catch (...) {
            std::cerr << "[DataReader] Failed to parse line: " << line << std::endl;
        }
    }

    return true;
}