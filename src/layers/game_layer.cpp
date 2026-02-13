#include "game_layer.hpp"
#include <ranges>

#include "states/combat/combatstate.hpp"

GameLayer::GameLayer(TacticalGame *ge)
{
    m_current_states.push_back(std::make_unique<PlayingState::CombatState>());
    m_current_states.front()->enter(ge);
}
GameLayer::~GameLayer() {}

void GameLayer::on_event(TacticalGame *ge, Event &event)
{
    EventDispatcher dispatcher(ge, event);
    m_current_states.back()->handle_input(ge, event);
}

void GameLayer::update(TacticalGame *ge)
{
    for (auto &state : std::views::reverse(m_current_states)) {
        state->update(ge);
    }
}
void GameLayer::draw(TacticalGame *ge)
{
    for (auto &state : std::views::reverse(m_current_states)) {
        state->draw(ge);
    }
}
bool GameLayer::key_released(TacticalGame *, KeyReleasedEvent &event) { return true; }
bool GameLayer::key_pressed(TacticalGame *, KeyPressedEvent &event) { return true; }

bool GameLayer::mouse_button_released(TacticalGame *, MouseButtonReleasedEvent &event) { return true; }
bool GameLayer::mouse_button_pressed(TacticalGame *, MouseButtonPressedEvent &event) { return true; }
