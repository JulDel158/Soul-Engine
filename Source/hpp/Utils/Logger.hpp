#ifndef LOGGER_HPP
#pragma once

#include "EngineDataStructures.hpp"

#include <fstream>
#include <string>
#include <mutex>
#include <chrono>

class Logger {
private:
    std::ofstream log_file_;
    std::mutex log_mutex_;
    ELogLevel min_level_;
	
public:
	Logger();
	explicit Logger(const std::string& filename, const ELogLevel level = ELogLevel::Info);
	~Logger();
	
	void Log(const ELogLevel level, const std::string& message);
	
private:
    static std::string LevelToString(const ELogLevel level);
    static std::string GetTimestamp();
};

#endif