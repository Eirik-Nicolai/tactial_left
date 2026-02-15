#include "menu_layer.hpp"

#include "engine/mouse_event.hpp"
#include "utils/geometry.hpp"

MenuLayer::MenuLayer(TacticalGame *ge) {}
MenuLayer::~MenuLayer() {}

void MenuLayer::on_event(TacticalGame *ge, Event &event)
{
    EventDispatcher dispatcher(ge, event);
    dispatcher.Dispatch<MouseButtonReleasedEvent>(
        [this](TacticalGame *ge, MouseButtonReleasedEvent &e) {
            return mouse_button_released(ge, e);
        });

    dispatcher.Dispatch<MouseButtonPressedEvent>(
        [this](TacticalGame *ge, MouseButtonPressedEvent &e) {
            return mouse_button_pressed(ge, e);
        });
}

void MenuLayer::update(TacticalGame *ge) {}
void MenuLayer::draw(TacticalGame *ge) { } //ge->FillRect({100, 100}, {300, 300}); }

bool MenuLayer::mouse_button_released(TacticalGame *ge, MouseButtonReleasedEvent &event)
{
    auto window_pos = olc::vf2d(100, 100);
    auto window_size = olc::vf2d(300, 300);
    auto mouse = ge->GetMousePos();
    if (is_point_inside_rect(window_pos.x, window_pos.y, window_size.x, window_size.y,
                             mouse.x, mouse.y)) {
        Info("Released menu square");
        return true;
    }
    return false;
}
bool MenuLayer::mouse_button_pressed(TacticalGame *ge, MouseButtonPressedEvent &event)
{
    auto window_pos = olc::vf2d(100, 100);
    auto window_size = olc::vf2d(300, 300);
    auto mouse = ge->GetMousePos();
    if (is_point_inside_rect(window_pos.x, window_pos.y, window_size.x, window_size.y,
                             mouse.x, mouse.y)) {
        Info("Pressed menu square");
        return true;
    }
    return false;
}
