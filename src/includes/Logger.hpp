#pragma once
#include <iostream>
#include <string_view>

enum class LogLevel { INFO, WARNING, ERROR, NOTE };

class Logger {
public:
    template <typename... Args>
    static void log(LogLevel level, Args&&... args) {
        switch (level) {
            case LogLevel::INFO:    std::cout << "[INFO] "; break;
            case LogLevel::WARNING: std::cout << "[WARNING] "; break;
            case LogLevel::ERROR:   std::cerr << "[ERROR] "; break;
            case LogLevel::NOTE:    std::cout << "[NOTE] "; break;
        }

        (std::cout << ... << args) << '\n';
    }

    template <typename... Args>
    static void info(Args&&... args) { log(LogLevel::INFO, std::forward<Args>(args)...); }

    template <typename... Args>
    static void warn(Args&&... args) { log(LogLevel::WARNING, std::forward<Args>(args)...); }

    template <typename... Args>
    static void error(Args&&... args) { log(LogLevel::ERROR, std::forward<Args>(args)...); }

    template <typename... Args>
    static void note(Args&&... args) { log(LogLevel::NOTE, std::forward<Args>(args)...); }
};