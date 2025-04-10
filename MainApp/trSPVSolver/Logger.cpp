#include "Logger.h"
#include <iostream>
#include <ctime>

std::ofstream Logger::log_file_;
std::mutex Logger::mutex_;

static std::string timestamp() {
    std::time_t t = std::time(nullptr);
    char buf[32];
    std::tm tm_buf;
    localtime_s(&tm_buf, &t);
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm_buf);
    return std::string(buf);
}

void Logger::init(const std::string& filepath) {
    std::lock_guard<std::mutex> lock(mutex_);
    log_file_.open(filepath, std::ios::out);
    if (log_file_) {
        log_file_ << "[Logger] Log started at " << timestamp() << "\n";
    }
}

void Logger::info(const std::string& msg) {
    std::lock_guard<std::mutex> lock(mutex_);
    std::cout << "[INFO] " << msg << std::endl;
    if (log_file_) log_file_ << "[INFO] " << msg << "\n";
}

void Logger::warn(const std::string& msg) {
    std::lock_guard<std::mutex> lock(mutex_);
    std::cout << "[WARN] " << msg << std::endl;
    if (log_file_) log_file_ << "[WARN] " << msg << "\n";
}

void Logger::error(const std::string& msg) {
    std::lock_guard<std::mutex> lock(mutex_);
    std::cerr << "[ERROR] " << msg << std::endl;
    if (log_file_) log_file_ << "[ERROR] " << msg << "\n";
}

void Logger::shutdown() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (log_file_) {
        log_file_ << "[Logger] Log ended at " << timestamp() << "\n";
        log_file_.close();
    }
}