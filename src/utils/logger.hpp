#pragma once

#include <string>
#include <iostream>

namespace MatrixTransform {

    enum class LogLevel {
        Debug,
        Info,
        Warning,
        Error,
        None
    };

    class Logger {
    public:
        static Logger& getInstance() {
            static Logger instance;
            return instance;
        }

        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;

        void setLevel(const std::string& levelStr);
        void log(LogLevel level, const std::string& message);

    private:
        Logger() : current_level_(LogLevel::Info) {}
        ~Logger() = default;

        LogLevel current_level_;
    };

} // namespace MatrixTransform