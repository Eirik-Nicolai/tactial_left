#pragma once
#include "engine/event.hpp"

class GameEvent : public Engine::Event
{
  public:
    GameEvent() {}

    EVENT_CLASS_TYPE(GameEvent)

  private:
};
