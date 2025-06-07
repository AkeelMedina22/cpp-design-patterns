#include "logger.hpp"
#include <chrono> 
#include <sstream>  
#include <iomanip>  

namespace MatrixTransform {

void Logger::setLevel(const std::string& levelStr) {
    if (levelStr == "debug") {
        current_level_ = LogLevel::Debug;
    } else if (levelStr == "info") {
        current_level_ = LogLevel::Info;
    } else if (levelStr == "warning") {
        current_level_ = LogLevel::Warning;
    } else if (levelStr == "error") {
        current_level_ = LogLevel::Error;
    } else if (levelStr == "none") {
        current_level_ = LogLevel::None;
    } else {
        log(LogLevel::Warning, "Unknown log level '" + levelStr + "' in config. Using default.");
    }
}

void Logger::log(LogLevel level, const std::string& message) {
    if (level >= current_level_) {
        const auto now = std::chrono::system_clock::now();
        const auto now_time_t = std::chrono::system_clock::to_time_t(now);

        std::stringstream time_stream;
        time_stream << std::put_time(std::localtime(&now_time_t), "%Y-%m-%d %H:%M:%S");
        const std::string time_str = time_stream.str();

        std::string level_str;
        switch (level) {
            case LogLevel::Debug:   level_str = "[DEBUG]";   break;
            case LogLevel::Info:    level_str = "[INFO]";    break;
            case LogLevel::Warning: level_str = "[WARNING]"; break;
            case LogLevel::Error:   level_str = "[ERROR]";   break;
            case LogLevel::None:    return; 
        }

        if (level >= LogLevel::Warning) {
            std::cerr << time_str << " " << level_str << " " << message << std::endl;
        } else {
            std::cout << time_str << " " << level_str << " " << message << std::endl;
        }
    }
}

} // namespace MatrixTransform