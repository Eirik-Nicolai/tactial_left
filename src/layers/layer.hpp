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
    Layer(TacticalGame* ge, std::shared_ptr<GameRegistry> reg) : m_registry(reg), m_game(ge) {}
    virtual ~Layer() = default;

    virtual void on_event(Engine::Event &event) {}

    virtual void update() {}
    virtual void draw() {}

    // 	template<std::derived_from<Layer> T, typename... Args>
    // 	void TransitionTo(Args&&... args)
    // 	{
    // 		QueueTransition(std::move(std::make_unique<T>(std::forward<Args>(args)...)));
    // 	}
  protected:
    std::shared_ptr<GameRegistry> m_registry;
    TacticalGame* m_game;
};
