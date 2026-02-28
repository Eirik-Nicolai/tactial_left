#pragma once

#include "olc/olcPixelGameEngine.h"
#include "olc/olcPGEX_FrostUI.h"

#include "layer.hpp"
#include "game.hpp"

#include "engine/event.hpp"
#include "engine/mouse_event.hpp"
class DebugLayer : public Layer
{
  public:
    GET_NAME(Layer, MenuLayer)

    DebugLayer(TacticalGame* ge, std::shared_ptr<GameRegistry> reg);
    virtual ~DebugLayer();

    virtual void on_event(Engine::Event &) override;
    virtual void update() override;
    virtual void draw() override;

  private:
    // bool key_released(KeyReleasedEvent &event);
    // bool key_pressed(KeyPressedEvent &event);

    bool mouse_button_released(Engine::MouseButtonReleasedEvent &event);
    bool mouse_button_pressed(Engine::MouseButtonPressedEvent &event);

    // TODO do later
    olc::FrostUI m_ui_manager;
};
