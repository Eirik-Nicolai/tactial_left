#pragma once
#include "engine/event.hpp"
namespace Engine {

class GameEvent : public Event
{
  public:
    GameEvent() {}

    EVENT_CLASS_TYPE(GameEvent)

  private:
};
};
