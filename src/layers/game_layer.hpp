#pragma once
#include "layer.hpp"
#include "engine/mouse_event.hpp"
#include "engine/key_event.hpp"

#include "states/gamestate.hpp"

class GameLayer : public Layer
{
  public:
    GET_NAME(Layer, GameLayer)

    GameLayer(TacticalGame *);
    virtual ~GameLayer();

    virtual void on_event(TacticalGame *, Engine::Event &event) override;

    virtual void update(TacticalGame *) override;
    virtual void draw(TacticalGame *) override;

  private:
    bool key_released(TacticalGame *, Engine::KeyReleasedEvent &event);
    bool key_pressed(TacticalGame *, Engine::KeyPressedEvent &event);

    bool mouse_button_released(TacticalGame *, Engine::MouseButtonReleasedEvent &event);
    bool mouse_button_pressed(TacticalGame *, Engine::MouseButtonPressedEvent &event);

    std::vector<std::unique_ptr<GameState>> m_current_states;
};
