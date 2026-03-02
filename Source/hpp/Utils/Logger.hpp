#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <string>
#include <mutex>
#include <chrono>

#include "EngineDataStructures.hpp"

class Logger {
private:
    std::ofstream log_file_;
    std::mutex log_mutex_;
    ELogLevel min_level_;

    // Convert LogLevel to string
    static std::string LevelToString(const ELogLevel level);

    // Get current timestamp
    std::string GetTimestamp();

public:
	Logger() = delete;
    explicit Logger(const std::string& filename, const ELogLevel level = ELogLevel::Warning);
	
    ~Logger();
	
    void Log(const ELogLevel level, const std::string& message);
};

#endif