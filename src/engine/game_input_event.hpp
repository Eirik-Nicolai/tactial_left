#pragma once
#include "engine/event.hpp"
namespace Engine {


    
class GameEvent : public Event
{
  public:
    GameEvent(std::string&& s) : m_type(std::forward<std::string>(s)) {}

    std::string_view get_type() { return m_type; }
    EVENT_CLASS_TYPE(GameEvent)

  private:
    std::string m_type;
};
};
