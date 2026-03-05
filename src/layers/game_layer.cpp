#include "game_layer.hpp"
#include "components/gui.hpp"
#include "engine/game_input_event.hpp"
#include "states/combat/combatstate.hpp"
#include "states/combat/combatstate_playermovement.hpp"

#include "registry/game_event.hpp"
#include "utils/geometry.hpp"

// void GameLayer::on_state_change(state_change new_state) {
//     Info("Geir Gustav --- " << new_state.geir_gustav.get_name());
// }

// void GameLayer::on_state_change_func(state_change_func f) {
//     auto new_state = f.geir_gustav();
//     Info("Geir Gustav " << new_state->get_name());
// }

void GameLayer::on_state_change_playaction(do_state_change_playaction d)
{
    Info("Changing to state - PlayerState");
    Debug("Erase current state");
    
    Debug("States " << m_current_states.size());
    m_current_states.erase(
        std::remove_if(m_current_states.begin(), m_current_states.end(),
                       [this](const std::unique_ptr<GameState> &x) {
                           Info(x->get_name());
                           return x->get_name() == "GameState::CombatstateMovement"; // HACK
                       }),
        m_current_states.end());

    
    Debug("States after " << m_current_states.size());
    Debug("Add state");
    m_current_states.push_back(std::move(
        std::make_unique<PlayingState::CombatStatePlayerAction>(m_game, m_registry)));
}

GameLayer::GameLayer(TacticalGame* ge, std::shared_ptr<GameRegistry> reg) : Layer(ge, reg)
{
    // TODO have issue around this state being the base state but also the loader so we load things several times
    m_current_states.push_back(
        std::move(std::make_unique<PlayingState::CombatState>(m_game, m_registry)));

    reg->dispatcher
        .add_listener_to_event<do_state_change_playaction,
                               &GameLayer::on_state_change_playaction>(this);
}

GameLayer::~GameLayer() {}

bool GameLayer::mouse_moved(Engine::MouseMovedEvent &event) {
    using namespace Component;
    auto reg = m_game->registry();
    auto mouse_pos =
        m_game->get_tv()->ScaleToWorld(m_game->GetMousePos()) + m_game->get_tv()->GetWorldOffset();
    for (auto &&[ent, hoverable] :
         m_registry->get().view<Component::Interaction::Hoverable>(entt::exclude<GUI::_is_gui>).each()) {
        const bool was_hovered = hoverable.is_hovered;
        // TODO is this the best way to do this
        hoverable.is_hovered = is_point_inside_rect(
            hoverable.boundaries.pos, hoverable.boundaries.size, mouse_pos);
        // do action if state changed
        if ((!was_hovered && hoverable.is_hovered) && hoverable.on_mouse_hover) {
            hoverable.on_mouse_hover(reg, ent);
        } else if ((was_hovered && !hoverable.is_hovered) && hoverable.on_mouse_exit) {
            hoverable.on_mouse_exit(reg, ent);
        }
    }
    return false;
}

void GameLayer::on_event(Engine::Event &event)
{
    Engine::EventDispatcher dispatcher(event);
    dispatcher.Dispatch<Engine::MouseMovedEvent>(
        [this](Engine::MouseMovedEvent &e) {
            return mouse_moved(e);
        });
    dispatcher.Dispatch<Engine::MouseButtonReleasedEvent>(
        [this](Engine::MouseButtonReleasedEvent &e) {
            return mouse_button_released(e);
        });
    dispatcher.Dispatch<Engine::KeyReleasedEvent>(
        [this](Engine::KeyReleasedEvent &e) {
            return key_released(e);
        });

    m_current_states.back()->handle_input(event);
}

void GameLayer::update()
{
    for (auto &state : m_current_states) {
        state->update();
    }
}
void GameLayer::draw()
{
    for (auto &state : m_current_states) {
        state->draw(m_game);
    }

    // HACK
    for (auto &&[ent, hoverable] :
         m_registry->get()
             .view<Component::Interaction::Hoverable>(entt::exclude<GUI::_is_gui>)
             .each()) {
        m_game->get_tv()->DrawRect(
            {hoverable.boundaries.pos.x, hoverable.boundaries.pos.y},
            {hoverable.boundaries.size.w, hoverable.boundaries.size.h},
            hoverable.is_hovered ? olc::BLUE : olc::DARK_BLUE);
    }
}
bool GameLayer::key_released(Engine::KeyReleasedEvent &event)
{
    if (m_current_states.size() == 1) {
        // TODO create helper function
        m_current_states.push_back(
            std::move(std::make_unique<PlayingState::CombatStatePlayerMovement>(
                m_game, m_registry)));
    } else {
        m_current_states.pop_back();
    }
    return false;
}
bool GameLayer::key_pressed(Engine::KeyPressedEvent &event) { return false; }

bool GameLayer::mouse_button_released(Engine::MouseButtonReleasedEvent &event)
{
    return false;
}
bool GameLayer::mouse_button_pressed(Engine::MouseButtonPressedEvent &event)
{
    return false;
}
