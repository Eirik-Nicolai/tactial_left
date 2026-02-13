#pragma once
#include "engine/event.hpp"
#include "game.hpp"
#include "utils/debugging.hpp"
#include "engine/input_handler.hpp"

class TacticalGame;
class Layer
{
  public:
    GET_BASE(Layer)

    virtual ~Layer() = default;

    virtual void on_event(TacticalGame *, Event &event) {}

    virtual void update(TacticalGame *) {}
    virtual void draw(TacticalGame *) {}

    // 	template<std::derived_from<Layer> T, typename... Args>
    // 	void TransitionTo(Args&&... args)
    // 	{
    // 		QueueTransition(std::move(std::make_unique<T>(std::forward<Args>(args)...)));
    // 	}
    // private:
    // 	void QueueTransition(std::unique_ptr<Layer> layer);
};
