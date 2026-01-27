#include "logger.hpp"

#include "spdlog/sinks/stdout_color_sinks.h"

Logger* Logger::m_state;

void Logger::set_log_level(spdlog::level::level_enum log_level) {

    m_logger = std::move(spdlog::stdout_color_mt("APP Logger"));

    if(log_level == spdlog::level::trace) {
        m_format = "[%^%L%$] (%uns) %v";
    } else {
        m_format = "[%^%L%$] %v";
    }
    m_logger->set_pattern(m_format);
    m_logger->set_level(log_level);

    log(spdlog::level::info, "Logger {} initialized", m_logger->name());
}
