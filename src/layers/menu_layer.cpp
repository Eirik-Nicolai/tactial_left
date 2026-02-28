#include "menu_layer.hpp"

#include "engine/mouse_event.hpp"
#include "utils/geometry.hpp"

MenuLayer::~MenuLayer() {}

void MenuLayer::on_event(Engine::Event &event)
{
    Engine::EventDispatcher dispatcher(event);
    dispatcher.Dispatch<Engine::MouseButtonReleasedEvent>(
        [this](Engine::MouseButtonReleasedEvent &e) {
            return mouse_button_released(e);
        });

    dispatcher.Dispatch<Engine::MouseButtonPressedEvent>(
        [this](Engine::MouseButtonPressedEvent &e) {
            return mouse_button_pressed(e);
        });
}

void MenuLayer::update() {}
void MenuLayer::draw() { } //ge->FillRect({100, 100}, {300, 300}); }

bool MenuLayer::mouse_button_released(Engine::MouseButtonReleasedEvent &event)
{
    auto window_pos = olc::vf2d(100, 100);
    auto window_size = olc::vf2d(300, 300);
    auto mouse = m_game->GetMousePos();
    if (is_point_inside_rect(window_pos.x, window_pos.y, window_size.x, window_size.y,
                             mouse.x, mouse.y)) {
        Info("Released menu square");
        return true;
    }
    return false;
}
bool MenuLayer::mouse_button_pressed(Engine::MouseButtonPressedEvent &event)
{
    auto window_pos = olc::vf2d(100, 100);
    auto window_size = olc::vf2d(300, 300);
    auto mouse = m_game->GetMousePos();
    if (is_point_inside_rect(window_pos.x, window_pos.y, window_size.x, window_size.y,
                             mouse.x, mouse.y)) {
        Info("Pressed menu square");
        return true;
    }
    return false;
}
