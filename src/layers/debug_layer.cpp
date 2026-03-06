#include "utils/geometry.hpp"
#define OLC_PGEX_FUI
#include "debug_layer.hpp"

// #define PGEX_UI
// #include "olc/olcPGEX_UI.h"

DebugLayer::DebugLayer(TacticalGame* ge, std::shared_ptr<GameRegistry> reg) : Layer(ge, reg)
{
    m_debug_lines["DBG"] = std::vector<std::string>{"DEBUG INFO"};
    m_debug_box.is_active = true;
    m_debug_box.pos = {100,300};
    m_debug_box.size = {100,20};

    m_dragging_debug_box = false;
    m_mouse_offs = {0,0};
}

DebugLayer::~DebugLayer() {}

void DebugLayer::update()
{
    auto box_width = 0;
    auto box_height = 0;
    for (auto [key, list] : m_debug_lines) {
        box_height += list.size() * LINE_HEIGHT + 4; // 2 pixel padding each side
        for (auto s : list) {
            auto printed_s = key + " - " + s;
            if (box_width < printed_s.length() * LINE_WIDTH) {
                box_width = (printed_s.length() * LINE_WIDTH); // 2 pixel padding each side
            }
        }
    }
    m_debug_box.size.x = box_width;
    m_debug_box.size.y = box_height;
}

void DebugLayer::draw()
{
    m_game->FillRect(m_debug_box.pos, m_debug_box.size+10, BG_COLOUR);
    m_game->DrawRect(m_debug_box.pos, m_debug_box.size+10, BORDER_COLOUR);

    auto string_y = m_debug_box.pos.y+4;
    for (auto [key, list] : m_debug_lines) {
        for (auto s : list) {
            auto printed_s = key + " - " + s;
            m_game->DrawString(m_debug_box.pos.x+5, string_y, printed_s);
            string_y += LINE_HEIGHT;
        }
    }
    
}

void DebugLayer::on_event(Engine::Event &e)
{
    using namespace Engine;
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<Engine::MouseMovedEvent>(
        [this](Engine::MouseMovedEvent &e) { return mouse_moved(e); });
    dispatcher.Dispatch<Engine::MouseButtonPressedEvent>(
        [this](Engine::MouseButtonPressedEvent &e) { return mouse_button_pressed(e); });
    dispatcher.Dispatch<Engine::MouseButtonReleasedEvent>(
        [this](Engine::MouseButtonReleasedEvent &e) { return mouse_button_released(e); });
    dispatcher.Dispatch<DebugEvent>([this](DebugEvent &e) { return update_debug(e); });
}
// bool DebugLayer::key_released(TacticalGame *, KeyReleasedEvent &event) {

// }
// bool DebugLayer::key_pressed(TacticalGame *, KeyPressedEvent &event) {

// }

bool DebugLayer::mouse_moved(Engine::MouseMovedEvent &event)
{
    auto get_name = []() { return "DebugLayer - mouse_button_moved()"; };

    if(m_dragging_debug_box) {
        auto [mx, my] = event.get_screen_pos();
        m_debug_box.pos = olc::vi2d(mx, my) - m_mouse_offs;
    }

    return false;
}
bool DebugLayer::mouse_button_released(Engine::MouseButtonReleasedEvent &event)
{
    auto get_name = []() { return "DebugLayer - mouse_button_released()"; };

    if(m_dragging_debug_box) {
        Info("Releasing debug box");
        m_dragging_debug_box = false;
        m_mouse_offs = {0,0};
        return true;
    }
    
    return false;
}
bool DebugLayer::mouse_button_pressed(Engine::MouseButtonPressedEvent &event)
{
    auto get_name = []() { return "DebugLayer - mouse_button_pressed()"; };

    if (event.get_button() == Engine::MouseButtonEvent::MouseButton::LeftMouseButton) {
        auto [mouse_x, mouse_y] = event.get_screen_pos();
        Debug("Checking {" << mouse_x << ", " << mouse_y << "} inside ["
                           << m_debug_box.pos << "  " << "," << m_debug_box.size << "]");

        if (is_point_inside_rect(m_debug_box.pos.x, m_debug_box.pos.y, m_debug_box.size.x,
                                 m_debug_box.size.y, mouse_x, mouse_y)) {
            Info("Grabbing debug box");
            m_dragging_debug_box = true;
            auto [mx, my] = event.get_screen_pos();
            m_mouse_offs = olc::vi2d{mx, my} - m_debug_box.pos;
            return true;
        }
    }

    return false;
}

bool DebugLayer::update_debug(Engine::DebugEvent &event)
{
    m_debug_lines[event.key()] = event.get_debug_list();
    
    return true;
}
