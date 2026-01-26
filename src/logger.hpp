#pragma once

#include "spdlog/spdlog-inl.h"

class Logger {
    /// ------ SINGLETON LOGIC ------ ///
    public:
        static Logger* Get() {
            if(!m_state)
                m_state = new Logger();
            return m_state;
        }
    private:
        static Logger* m_state;
        Logger() {}

    public:
        void set_log_level(spdlog::level::level_enum log_level);
        bool level() { return m_logger->level(); }

        template <typename ... Args>
        void log(spdlog::level::level_enum level,
                 spdlog::format_string_t<Args...> fmt, Args && ...args) {
            m_logger->log(level, fmt, std::forward<Args>(args) ...);
        }

    private:
        std::shared_ptr<spdlog::logger> m_logger;
        std::string m_format;
};

using namespace spdlog::level;
#define PRINT_FUNC(x)                                                    \
    if(Logger::Get()->level() > x) {                                     \
        std::cout << "--> [" << get_name() << "::" << __func__ << "()]"; \
    }

#define LOG(_1, _2, ...) \
    Logger::Get()->log(_1, _2, __VA_ARGS__)

#define Error(_1, ...) LOG(spdlog::level::err,  _1, __VA_ARGS__)
#define Warn(_1, ...)  LOG(spdlog::level::warn, _1, __VA_ARGS__)
#define Info(_1, ...)  LOG(spdlog::level::info, _1, __VA_ARGS__)
#define Debug(_1, ...) LOG(spdlog::level::debug,_1, __VA_ARGS__)
#define Trace(_1, ...) LOG(spdlog::level::trace,_1, __VA_ARGS__)


#define LOG_FUNC                       \
    PRINT_FUNC(spdlog::level::trace)   \
    std::cout << std::endl;
