#pragma once
#include "logger_level.hpp"

// #include "spdlog/spdlog.h"

// TODO change to separate thread ostring operator overload

// using namespace spdlog::level;
#define PRINT_FUNC(x)                                                                    \
    if (Logger::get().loglevel() > Logger::LogLevel::DEBUG) {                            \
    } else                                                                               \
        std::cout << "[" << get_name() << "::" << __func__ << "()]" << std::endl;

#define LOG(_1, _2, ...)                                                                 \
    std::cout<<std::boolalpha << _1 << " (" << get_name() << ") - " << _2 << std::endl;
// Logger::Get()->log(_1, _2, ##__VA_ARGS__)

#define LOG_ERR()        "[\033[31mERR\033[0m]  "
#define LOG_WARN(_1)     "[\033[33mWARN\033[0m] "
#define LOG_INFO(_1)     "[\033[32mINFO\033[0m] "
#define LOG_DEBUG(_1)    "[\033[35mDEBUG\033[0m]"
#define LOG_TRACE(_1)    "[\033[37mTRACE\033[0m]"

#define Error(_1, ...)                                                                   \
    if (Logger::get().loglevel() > Logger::LogLevel::ERROR) {                            \
    } else                                                                               \
        LOG(LOG_ERR(), _1)
#define Warn(_1, ...)                                                                    \
    if (Logger::get().loglevel() > Logger::LogLevel::ERROR) {                            \
    } else                                                                               \
        LOG(LOG_WARN(), _1)
#define Info(_1, ...)                                                                     \
    if (Logger::get().loglevel() > Logger::LogLevel::INFO) {                            \
    } else                                                                               \
        LOG(LOG_INFO(), _1)
#define Debug(_1, ...)                                                                    \
    if (Logger::get().loglevel() > Logger::LogLevel::DEBUG) {                            \
    } else                                                                               \
        LOG(LOG_DEBUG(), _1)
#define Trace(_1, ...)                                                                    \
    if (Logger::get().loglevel() > Logger::LogLevel::TRACE) {                            \
    } else                                                                               \
        LOG(LOG_TRACE(), _1)

#define LOG_FUNC PRINT_FUNC()
