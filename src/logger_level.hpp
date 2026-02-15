#pragma once
#include <iostream>
#include <magic_enum.hpp>

// TODO change to separate thread ostring operator overload
class Logger
{
  public:
    enum class LogLevel { TRACE = 0, DEBUG, INFO, ERROR };

  public:
    /// ------ SINGLETON LOGIC ------ ///
    static Logger &get()
    {
        static Logger instance;
        return instance;
    }

  public:
    Logger(Logger const &) = delete;
    void operator=(Logger const &) = delete;

    void set_log_level(std::string level)
    {
        m_level =
            magic_enum::enum_cast<LogLevel>(level, magic_enum::case_insensitive).value();
    }
    void set_log_level(int level)
    {
        try {
            m_level = static_cast<LogLevel>(level);
        } catch (std::runtime_error &e) {
            std::cerr << "Unable to set loglevel !" << std::endl;
            throw(e);
        }
    }
    void set_log_level(LogLevel level) { m_level = level; }

    LogLevel loglevel() { return m_level; }

  private:
    Logger() {}
    LogLevel m_level = LogLevel::INFO;
};
