#include "Utils/Logger.hpp"

#include <iostream>
#include <ctime>
#include <sstream>

namespace
{
	constexpr unsigned int TIMESTAMP_BUFFER_SIZE = 20;
}

std::string Logger::LevelToString(const ELogLevel level)
{
	switch (level) 
	{
	case ELogLevel::Info:
		{
			return "INFO";
		}
    case ELogLevel::Warning:
		{
			return "WARNING";
		}
    case ELogLevel::Error:
		{
			return "ERROR";
		}
    case ELogLevel::Debug:
		{
			return "DEBUG";
		}
    default: // NOLINT (casted value could be passed triggering this case)
		{
			return "UNKNOWN";
		}
    }
}

std::string Logger::GetTimestamp()
{
	auto now = std::chrono::system_clock::now();
	std::time_t nowC = std::chrono::system_clock::to_time_t(now);
	std::tm tm{};
#if defined(_WIN32) || defined(_WIN64)
	static_cast<void>(localtime_s(&tm, &nowC));
#else
	static_cast<void>(localtime_r(&nowC, &tm));
#endif
	char buffer[TIMESTAMP_BUFFER_SIZE];
	static_cast<void>(std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tm));
	return buffer;
}

Logger::Logger(const std::string& filename, const ELogLevel level) :
min_level_(level)
{
	log_file_.open(filename, std::ios::app);
	if (!log_file_)
	{
		throw std::runtime_error("Failed to open log file: " + filename);
	}
}

Logger::~Logger()
{
	if (log_file_.is_open())
	{
		log_file_.close();
	}
}

void Logger::Log(const ELogLevel level, const std::string& message)
{
	if (level < min_level_ || !log_file_.is_open())
	{
		return; // Skip messages below min level
	}

    std::lock_guard<std::mutex> lock(log_mutex_);  // NOLINT (modernize-use-scoped-lock contains unnecessary overhead for use case)
    std::ostringstream logEntry;
    logEntry << "[" << GetTimestamp() << "] "
		<< "[" << LevelToString(level) << "] "
		<< message << "\n";

	// Write to file
	log_file_ << logEntry.str();
	log_file_.flush();

	// Also print to console for ERROR and WARNING
	if (level == ELogLevel::Error || level == ELogLevel::Warning) 
	{
		std::cerr << logEntry.str();
	}
}
