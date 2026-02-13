#include "menu_layer.hpp"

#include "engine/mouse_event.hpp"

MenuLayer::MenuLayer(TacticalGame* ge) {}
MenuLayer::~MenuLayer() {}

void MenuLayer::on_event(TacticalGame *ge, Event &event) {
    EventDispatcher dispatcher(ge, event);
	dispatcher.Dispatch<MouseButtonPressedEvent>([this](TacticalGame* ge, MouseButtonPressedEvent& e) { return mouse_button_pressed(ge, e); });
	dispatcher.Dispatch<MouseButtonReleasedEvent>([this](TacticalGame* ge, MouseButtonReleasedEvent& e) { return mouse_button_released(ge, e); });
}

void MenuLayer::update(TacticalGame *) {}
void MenuLayer::draw(TacticalGame *) {}
bool MenuLayer::key_released(TacticalGame *, KeyReleasedEvent &event) {
  return true;
}
bool MenuLayer::key_pressed(TacticalGame *, KeyPressedEvent &event) {
  Error("Mouse button pressed !");
  return false;
}

bool MenuLayer::mouse_button_released(TacticalGame *,
                                      MouseButtonReleasedEvent &event) {
    Error("Mouse button released !");
    return false;
}
bool MenuLayer::mouse_button_pressed(TacticalGame *,
                                     MouseButtonPressedEvent &event) {
  Error("Mouse button pressed !");
  return false;
}
