#pragma once

#include <string>
#include <fstream>
#include <mutex>

class Logger {
public:
    static void init(const std::string& filepath);
    static void info(const std::string& msg);
    static void warn(const std::string& msg);
    static void error(const std::string& msg);
    static void shutdown();

private:
    static std::ofstream log_file_;
    static std::mutex mutex_;
};