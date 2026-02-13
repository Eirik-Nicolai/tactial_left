#pragma once

#include "olc/olcPixelGameEngine.h"
#include "olc/olcPGEX_FrostUI.h"

#include "layer.hpp"
#include "game.hpp"

class DebugLayer : public Layer {
public:
    GET_NAME(Layer, MenuLayer)

    DebugLayer(TacticalGame* ge);
    virtual ~DebugLayer();

    virtual void update(TacticalGame *) override;
    virtual void draw(TacticalGame *) override;

  private:
    // bool key_released(TacticalGame *, KeyReleasedEvent &event);
    // bool key_pressed(TacticalGame *, KeyPressedEvent &event);

    // bool mouse_button_released(TacticalGame *, MouseButtonReleasedEvent
    // &event); bool mouse_button_pressed(TacticalGame *,
    // MouseButtonPressedEvent &event);

    // TODO do later
    olc::FrostUI m_ui_manager;
};
