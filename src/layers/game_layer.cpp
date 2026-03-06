#include "game_layer.hpp"
#include "components/gui.hpp"
#include "engine/game_input_event.hpp"
#include "states/combat/combatstate.hpp"
#include "states/combat/combatstate_initialize.hpp"

#include "registry/game_event.hpp"
#include "utils/geometry.hpp"

#include <ranges>

GameLayer::GameLayer(TacticalGame* ge, std::shared_ptr<GameRegistry> reg) : Layer(ge, reg)
{
    // TODO have issue around this state being the base state but also the loader so we load things several times
    add_state(std::move(std::make_unique<PlayingState::CombatState>(m_game, m_registry)));
    add_state(std::move(std::make_unique<TransitionState::CombatStateInit>(m_game, m_registry)));
}

GameLayer::~GameLayer() {}

bool GameLayer::mouse_moved(Engine::MouseMovedEvent &event) {
    using namespace Component;
    auto [mouse_pos_x,mouse_pos_y] = event.get_world_pos();
    for (auto &&[ent, hoverable] :
         m_registry->get().view<Component::Interaction::Hoverable>(entt::exclude<GUI::_is_gui>).each()) {
        const bool was_hovered = hoverable.is_hovered;
        // TODO is this the best way to do this
        hoverable.is_hovered =
            is_point_inside_rect(hoverable.boundaries.pos, hoverable.boundaries.size,
                                 olc::vi2d(mouse_pos_x, mouse_pos_y));
        // do action if state changed
        if ((!was_hovered && hoverable.is_hovered) && hoverable.on_mouse_hover) {
            hoverable.on_mouse_hover(m_registry, ent);
        } else if ((was_hovered && !hoverable.is_hovered) && hoverable.on_mouse_exit) {
            hoverable.on_mouse_exit(m_registry, ent);
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

    // should only dod the latest state but this makes
    // debugging easier due to camera movement
    for (auto &state : std::views::reverse(m_current_states)) {
        state->handle_input(event);
        if (event.consumed)
            break;
    }

    // m_current_states.back()->handle_input(event);
}

void GameLayer::update()
{
    for (auto &state : m_current_states) {
        state->update();
    }

    if(!m_queued_states.empty()) {
        Debug(m_queued_states.size() << " queued states");
        while(!m_queued_states.empty()) {
            auto q = std::move(m_queued_states.back());
            change_state(q.index, std::move(q.queued_state));
            m_queued_states.pop_back();
        }
    }
}

void GameLayer::draw()
{
    for (auto &state : m_current_states) {
        state->draw();
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

void GameLayer::queue_state_change(int indx, std::unique_ptr<GameState> new_state) {
    Debug("queueing state at index " << indx);
    m_queued_states.emplace_back(queued_state{
        .index = indx,
        .queued_state = std::move(new_state)
    });
}
void GameLayer::change_state(int indx, std::unique_ptr<GameState> new_state) {
    Debug("Changing state at index " << indx);
    if(indx <= m_current_states.size()) {
        m_current_states[indx].swap(new_state);
    }

    m_current_states[indx]->connect_signals(
        indx,
        [this](do_state_change_add_overlay e) { add_state(std::move(e.new_state)); },
        [this](do_state_change_current e) {
            queue_state_change(e.index, std::move(e.new_state));
        });
    
    std::vector<std::string> states_list;
    for(auto &s : m_current_states) {
        states_list.emplace_back(s->get_name());
    }
    auto event = std::make_shared<Engine::DebugEvent>("Game", std::move(states_list));
    m_game->raise_event(event);
}
void GameLayer::add_state(std::unique_ptr<GameState> new_state) {
    Debug("Adding state at index " << m_current_states.size());
    m_current_states.push_back(std::move(new_state));

    m_current_states[m_current_states.size() - 1]->connect_signals(
        m_current_states.size() - 1,
        [this](do_state_change_add_overlay e) { add_state(std::move(e.new_state)); },
        [this](do_state_change_current e) {
            queue_state_change(e.index, std::move(e.new_state));
        });

    std::vector<std::string> states_list;
    for(auto &s : m_current_states) {
        states_list.emplace_back(s->get_name());
    }
    auto event = std::make_shared<Engine::DebugEvent>("Game", std::move(states_list));
    m_game->raise_event(event);
}
