#pragma once
#include "layer.hpp"
#include "engine/mouse_event.hpp"
#include "engine/key_event.hpp"
#include "utils/debugging.hpp"

class MenuLayer : public Layer
{
  public:
    GET_NAME(Layer, MenuLayer)

    MenuLayer(TacticalGame* ge, std::shared_ptr<GameRegistry> reg) : Layer(ge, reg) {}
    virtual ~MenuLayer();

    virtual void on_event(Engine::Event &) override;
    virtual void update() override;
    virtual void draw() override;

  private:
    bool key_released(Engine::KeyReleasedEvent &event);
    bool key_pressed(Engine::KeyPressedEvent &event);

    bool mouse_button_released(Engine::MouseButtonReleasedEvent &event);
    bool mouse_button_pressed(Engine::MouseButtonPressedEvent &event);
};
