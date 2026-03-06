#pragma once

#include "olc/olcPixelGameEngine.h"
#include "olc/olcPGEX_FrostUI.h"

#include "layer.hpp"
#include "game.hpp"

#include "engine/event.hpp"
#include "engine/mouse_event.hpp"

#define MY_INDEX 0
#define GAME_INDEX 1
#define GAME_LAYER_INDEX 2
#define INPUT_LAYER_INDEX 3
#define STATE_INDEX 4

class DebugLayer : public Layer
{
  public:
    GET_NAME(Layer, DebugLayer)

    DebugLayer(TacticalGame* ge, std::shared_ptr<GameRegistry> reg);
    virtual ~DebugLayer();

    virtual void on_event(Engine::Event &) override;
    virtual void update() override;
    virtual void draw() override;

  private:
    bool update_debug(Engine::DebugEvent &event);

    bool mouse_button_released(Engine::MouseButtonReleasedEvent &event);
    bool mouse_button_pressed(Engine::MouseButtonPressedEvent &event);

    bool mouse_moved(Engine::MouseMovedEvent &event);
    
    struct DebugBox{
        bool is_active;
        olc::vi2d pos;
        olc::vi2d size;
    } m_debug_box;

    bool m_dragging_debug_box;
    olc::vi2d m_mouse_offs;
    std::map<std::string, std::vector<std::string>> m_debug_lines;

    // consts
    const int INACTIVE_BOX_W = 15;
    const int INACTIVE_BOX_H = 10;
    const float BG_ALPHA = 40;
    const olc::Pixel BG_COLOUR = olc::Pixel(255,255,255,BG_ALPHA);
    const olc::Pixel BORDER_COLOUR = olc::DARK_MAGENTA;
    const int LINE_HEIGHT = 10;
    const int LINE_WIDTH = 8;
};
