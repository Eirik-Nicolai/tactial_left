#pragma once
#include "engine/event.hpp"
namespace Engine {


class DebugEvent : public Event
{
  public:
    DebugEvent(std::string indx, std::vector<std::string> &&debug_list)
    : m_index(indx), m_debug(std::forward<std::vector<std::string>>(std::move(debug_list)))
    {
    }

    std::string key() { return m_index; }
    std::vector<std::string> get_debug_list() { return m_debug; }
    EVENT_CLASS_TYPE(DebugEvent)

  private:
    std::vector<std::string> m_debug;
    std::string m_index;
};
};
