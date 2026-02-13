#pragma once

// #include "spdlog/spdlog.h"

// TODO change to self method to allow custom formatting of f. ex. components
// or find an example of the fucking formatter for spdlog
//

// class Logger {
//     /// ------ SINGLETON LOGIC ------ ///
//     public:
//         static Logger* Get() {
//             if(!m_state)
//                 m_state = new Logger();
//             return m_state;
//         }
//     private:
//         static Logger* m_state;
//         Logger() {}

//     public:
//         void set_log_level(spdlog::level::level_enum log_level);
//         bool level() { return m_logger->level(); }

//         template <typename ... Args>
//         void log(spdlog::level::level_enum level,
//                  spdlog::format_string_t<Args...> fmt, Args && ...args) {
//             m_logger->log(level, fmt, std::forward<Args>(args) ...);
//         }

//     private:
//         std::shared_ptr<spdlog::logger> m_logger;
//         std::string m_format;
// };

// using namespace spdlog::level;
#define PRINT_FUNC(x)                                                                    \
    std::cout << "[" << get_name() << "::" << __func__ << "()]" << std::endl;            \
    // if(Logger::Get()->level() <= x) {                                     \
    // }

#define LOG(_1, _2, ...)                                                                 \
    std::cout << "[" << _1 << "]" << get_name() << " - " << _2 << std::endl;
// Logger::Get()->log(_1, _2, ##__VA_ARGS__)

#define Error(_1, ...) LOG("err", _1)
#define Warn(_1, ...) LOG("warn", _1)
#define Info(_1, ...) LOG("info", _1)
#define Debug(_1, ...) LOG("debug", _1)
#define Trace(_1, ...) LOG("trace", _1)

#define LOG_FUNC PRINT_FUNC(spdlog::level::trace)
