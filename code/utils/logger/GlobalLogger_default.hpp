#pragma once
#ifdef _WIN32
#include <windows.h>
#endif
#include <memory>
#include <iostream>
#include <Logger.hpp>

enum class DefaultLevel {
    Debug = 0,
    Info,
    Warn,
    Error
};

struct DefaultInfo {

};

namespace globalLogger {
    inline DefaultLevel _level{DefaultLevel::Info};

    inline bool levelFilter(const Logger<DefaultLevel, DefaultInfo>::LogRecord& record) {
        return record.level >= _level;
    }

    inline std::string format(const Logger<DefaultLevel, DefaultInfo>::LogRecord& record) {
        std::string levelStr{};
        switch (record.level) {
            case DefaultLevel::Debug: levelStr = "Debug"; break;
            case DefaultLevel::Info: levelStr = "Info"; break;
            case DefaultLevel::Warn: levelStr = "Warn"; break;
            case DefaultLevel::Error: levelStr = "Error"; break;
        }
        return "[" + levelStr + "]: " + record.message;
    }

    inline void consoleHandler(const Logger<DefaultLevel, DefaultInfo>::LogRecord& record, const std::string& str) {
        #ifdef _WIN32
            // Windows 控制台颜色
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            WORD originalColor;
            CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
            GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
            originalColor = consoleInfo.wAttributes;

            switch (record.level) {
                case DefaultLevel::Error:
                    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
                    break;
                case DefaultLevel::Warn:
                    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                    break;
                case DefaultLevel::Info:
                    SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);
                    break;
                case DefaultLevel::Debug:
                    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN);
                    break;
            }

            std::cout << str << std::endl;
            SetConsoleTextAttribute(hConsole, originalColor);
        #else
            // Linux/Mac ANSI颜色
            switch (record.level) {
                case DefaultLevel::Error:
                    std::cout << "\033[1;31m" << str << "\033[0m" << std::endl;
                    break;
                case DefaultLevel::Warn:
                    std::cout << "\033[1;33m" << str << "\033[0m" << std::endl;
                    break;
                case DefaultLevel::Info:
                    std::cout << "\033[1;32m" << str << "\033[0m" << std::endl;
                    break;
                case DefaultLevel::Debug:
                    std::cout << "\033[36m" << str << "\033[0m" << std::endl;
                    break;
            }
        #endif
    }
};

inline Logger<DefaultLevel, DefaultInfo> glog = Logger<DefaultLevel, DefaultInfo>::builder()
    .appendFilter(globalLogger::levelFilter)
    .formatter(globalLogger::format)
    .appendHandler(globalLogger::consoleHandler)
    .build();