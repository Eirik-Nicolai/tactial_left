#include "game_layer.hpp"
#include <ranges>

#include "states/combat/combatstate.hpp"

GameLayer::GameLayer(TacticalGame *ge)
{
    m_current_states.push_back(std::move(std::make_unique<PlayingState::CombatState>()));   
    m_current_states.front()->enter(ge);
}
GameLayer::~GameLayer() {}

void GameLayer::on_event(TacticalGame *ge, Event &event)
{
    EventDispatcher dispatcher(ge, event);
    dispatcher.Dispatch<MouseButtonReleasedEvent>(
        [this](TacticalGame *ge, MouseButtonReleasedEvent &e) {
            return mouse_button_released(ge, e);
        });
    dispatcher.Dispatch<KeyReleasedEvent>(
        [this](TacticalGame *ge, KeyReleasedEvent &e) {
            return key_released(ge, e);
        });
    m_current_states.back()->handle_input(ge, event);
}

void GameLayer::update(TacticalGame *ge)
{
    for (auto &state : m_current_states) {
        state->update(ge);
    }
}
void GameLayer::draw(TacticalGame *ge)
{
    for (auto &state : m_current_states) {
        state->draw(ge);
    }
}
bool GameLayer::key_released(TacticalGame *, KeyReleasedEvent &event)
{
    if(m_current_states.size() == 1) {
        m_current_states.push_back(
            std::move(std::make_unique<PlayingState::CombatStateSelect>()));
    } else {
        m_current_states.pop_back();
    }
    return false;
}
bool GameLayer::key_pressed(TacticalGame *, KeyPressedEvent &event) { return false; }

bool GameLayer::mouse_button_released(TacticalGame *ge, MouseButtonReleasedEvent &event)
{
    return false;
}
bool GameLayer::mouse_button_pressed(TacticalGame *, MouseButtonPressedEvent &event)
{
    return false;
}
