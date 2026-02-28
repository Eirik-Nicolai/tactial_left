#pragma once
#include "layer.hpp"
#include "engine/mouse_event.hpp"
#include "engine/key_event.hpp"

#include "states/gamestate.hpp"

#include "registry/game_event.hpp"

class GameLayer : public Layer
{
  public:
    GET_NAME(Layer, GameLayer)

    GameLayer(TacticalGame*, std::shared_ptr<GameRegistry> reg);
    virtual ~GameLayer();

    virtual void on_event(Engine::Event &) override;
    virtual void update() override;
    virtual void draw() override;

  private:
    bool key_released(Engine::KeyReleasedEvent &event);
    bool key_pressed(Engine::KeyPressedEvent &event);

    bool mouse_button_released(Engine::MouseButtonReleasedEvent &event);
    bool mouse_button_pressed(Engine::MouseButtonPressedEvent &event);

    // void on_state_change(state_change);
    // void on_state_change_func(state_change_func);
    void on_state_change_playaction(do_state_change_playaction);

    std::vector<std::unique_ptr<GameState>> m_current_states;
};
