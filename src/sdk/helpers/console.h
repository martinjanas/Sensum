#pragma once

#define NOMINMAX
#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>
#include <tchar.h>
#include <cstdarg>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <list>

enum ELogLevel
{
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_DEBUG
};

class Console
{
public:
    Console()
    {
        attach();
    }
    //Default: LOG_INFO
    template <ELogLevel log_level = LOG_INFO>
    void println(const char* format, ...)
    {
        va_list args;
        va_start(args, format);

        char buf[512];
        vsnprintf_s(buf, sizeof(buf), _TRUNCATE, format, args);

        std::string log_level_prefix;
        switch (log_level)
        {
        case ELogLevel::LOG_INFO:
            log_level_prefix = "[INF]: ";
            break;
        case ELogLevel::LOG_WARNING:
            log_level_prefix = "[WRN]: ";
            break;
        case ELogLevel::LOG_ERROR:
            log_level_prefix = "[ERR]: ";
            break;
        case ELogLevel::LOG_DEBUG:
            log_level_prefix = "[DBG]: ";
            break;
        }

        printf_s("%s%s\n", log_level_prefix.c_str(), buf);

        std::string log_string = buf;

        if (!log_string.empty() && log_string.back() == '\n')
            log_string.pop_back();

        auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);

        std::tm now_tm;
        localtime_s(&now_tm, &now_time);

        std::ostringstream time_str;
        time_str << std::put_time(&now_tm, "[%H:%M:%S]");
        
        log_string = time_str.str() + log_level_prefix + log_string;

        log_output(log_string.c_str());
        
        va_end(args);
    }

    void detach();
    void save_log();
private:
    void attach();
    void log_output(const char* msg);

    std::list<std::string> log_data;
};
extern inline Console* g_Console = nullptr;


