#include "logger.hpp"

#include "spdlog/sinks/stdout_color_sinks.h"

Logger* Logger::m_state;

void Logger::set_log_level(spdlog::level::level_enum log_level) {

    m_logger = std::move(spdlog::stdout_color_mt("some_unique_name"));

    m_format = "[%^%L%$] (%uns) %v";
    m_logger->set_pattern(m_format);

    m_logger->log(spdlog::level::info,"Logger Initialized");

    m_logger->set_level(log_level);
}
