#include "combatstate_playermovement.hpp"
#include "combatstate_playeraction.hpp"
#include "logger.hpp"
#include "utils/geometry.hpp"
#include "registry/game_event.hpp"

namespace PlayingState
{

CombatStatePlayerMovement::CombatStatePlayerMovement(TacticalGame *ge,
                                                     std::shared_ptr<GameRegistry> reg)
    : CombatState(ge, reg)
{
    LOG_FUNC
    // TODO can use this for mapping buttons ig
    // if we're using buttons ?
    handler = std::make_unique<InputHandler>();
    m_controlled_entity = entt::null;
    m_tile_dest = entt::null;

    for (auto &&[ent] :
         m_registry->get().view<Component::Combat::Interaction::_Playable>().each()) {
        m_controlled_entity = ent;
    }
    Debug("Setting entity " << m_registry->entity_name(m_controlled_entity));

    for (auto &&[ent, holding] :
         m_registry->get().view<Component::Combat::CurrentlyHolding>().each()) {
        // HACK testing
        if (holding.value == m_controlled_entity) {
            m_registry->add_tag<Component::Combat::_Node_Start>(ent);
        }
    }

    Debug("Adding node callbacks");
    for (auto &&[ent, holding] :
         m_registry->get().view<Component::Combat::Node>().each()) {
        auto selectable =
            m_registry->get_component<Component::Interaction::Selectable>(ent);
        selectable->on_left_select = [this, name = m_registry->entity_name(ent)](std::shared_ptr<GameRegistry> reg,
                                           entt::entity me) {
            auto get_name = [name]() { return name; };
                Debug("Released");
                Component::Combat::Moving moving_sequence;
                auto pos = reg->get_component<Component::Pos>(m_controlled_entity);

                entt::entity start_tile_entity =
                    reg->get_entity_on_check<Component::Combat::CurrentlyHolding>(
                        [this](Component::Combat::CurrentlyHolding c) {
                            return m_controlled_entity == c.value;
                        });

                Debug("Starting at " << reg->entity_name(start_tile_entity));

                moving_sequence.moving_speed = 4;
                moving_sequence.directional_speed = {0, 0};
                auto traversing_entity = m_tile_dest;
                while (traversing_entity != entt::null) {
                    moving_sequence.dest_sequence.push_back(
                        reg->unsafe_get_component<Component::Pos>(traversing_entity));
                    traversing_entity =
                        reg->unsafe_get_component<Component::Combat::Node>(traversing_entity)
                            .parent;
                }

                moving_sequence.sequence_step = moving_sequence.dest_sequence.size() - 1;

                moving_sequence.on_reached_dest =
                    [this, start_tile_entity](std::shared_ptr<GameRegistry> reg) {
                        auto get_name = [&, start_tile_entity]() {
                            return "On Dest Reached !";
                        };
                        Info("Reached dest ! Removing from " << start_tile_entity);
                        reg->unsafe_remove_component<Component::Combat::CurrentlyHolding>(
                            start_tile_entity);
                        reg->add_component<Component::Combat::CurrentlyHolding>(m_tile_dest,
                                                                     m_controlled_entity);

                        reg->unsafe_remove_component<Component::Combat::Moving>(m_controlled_entity);
                        m_change_state_to<CombatStatePlayerAction>();
                    };  
                reg->unsafe_add_component<Component::Combat::Moving>(
                    m_controlled_entity, moving_sequence);

                return true;
        };
        selectable->on_right_select =
            [this, name = m_registry->entity_name(ent)](std::shared_ptr<GameRegistry> reg,
                                                 entt::entity me) {
                auto get_name = [name]() { return name; };
                auto node = reg->unsafe_get_component<Component::Combat::Node>(me);
                node.is_obstacle = !node.is_obstacle;

                return true;
            };

        auto p = m_registry->unsafe_get_component<Component::Pos>(ent);
        Component::Combat::World::TileInfo info;
        m_registry->get_world_component(info);
        m_registry->add_component<Component::Interaction::Hoverable>(
            ent,
            Component::Interaction::Hoverable{
                .boundaries = Component::Box{Component::Pos{p.x + 5, p.y + 5},
                                             Component::Size{.w = info.s.w - 5, .h = info.s.h - 5}},
                .is_hovered = false,
                // TODO later
                .on_mouse_hover =
                    [this, name = m_registry->entity_name(ent)](std::shared_ptr<GameRegistry> reg,
                                            entt::entity me) {
                        auto get_name = [name]() { return name; };
                        reg->remove_all_instances_of_tag<Component::Combat::_Node_End>();
                        reg->add_tag<Component::Combat::_Node_End>(me);
                        m_tile_dest = me; // TODO remove this member and just use _Node_End
                        solve_a_star();
                        return false;
                    },
                .on_mouse_exit =
                    [this, name = m_registry->entity_name(ent)](std::shared_ptr<GameRegistry> reg,
                                            entt::entity me) {
                        auto get_name = [name]() { return name; };
                        return false;
                    },
            });
    }

    Info("Finished setting up movement state");
}

void CombatStatePlayerMovement::handle_input(Engine::Event &event)
{
    Engine::EventDispatcher dispatcher(event);
    
    dispatcher.Dispatch<Engine::MouseButtonPressedEvent>(
        [this](Engine::MouseButtonPressedEvent &e) {
            return mouse_button_pressed(e);
        });
    dispatcher.Dispatch<Engine::MouseButtonReleasedEvent>(
        [this](Engine::MouseButtonReleasedEvent &e) {
            return mouse_button_released(e);
        });
    dispatcher.Dispatch<Engine::MouseMovedEvent>(
        [this](Engine::MouseMovedEvent &e) {
            return mouse_button_moved(e);
        });
}

void CombatStatePlayerMovement::update()
{
    // move entity to selected spot, one line at a time
    using namespace Component;

    for (auto &&[ent, pos, seq] :
         m_registry->get().view<Component::Pos, Combat::Moving>().each()) {
        // if we've reached next step in the sequence
        if(seq.sequence_step > seq.dest_sequence.size()) Error("out of bounds")
        if (pos == seq.dest_sequence[seq.sequence_step]) {
            // have we arrived
            if (seq.sequence_step == 0) {
                // seq.on_reached_dest(m_registry);
                entt::entity start_tile_entity =
                    m_registry->get_entity_on_check<Component::Combat::CurrentlyHolding>(
                        [this](Component::Combat::CurrentlyHolding c) {
                            return m_controlled_entity == c.value;
                        });
                auto get_name = []() { return "On Dest Reached !"; };
                Info("Reached dest ! Removing from " << start_tile_entity);
                m_registry->unsafe_remove_component<Component::Combat::CurrentlyHolding>(
                    start_tile_entity);
                m_registry->add_component<Component::Combat::CurrentlyHolding>(
                    m_tile_dest, m_controlled_entity);

                m_registry->unsafe_remove_component<Component::Combat::Moving>(
                    m_controlled_entity);
                m_change_state_to<CombatStatePlayerAction>();
                continue;
            }
            // else, incremnt and find new speed
            seq.sequence_step--;
            auto direction = seq.dest_sequence[seq.sequence_step+1] -
                             seq.dest_sequence[seq.sequence_step];

            direction.x = std::abs(direction.x);
            direction.y = std::abs(direction.y);
            Component::Combat::World::TileInfo info;
            m_registry->get_world_component(info);

            seq.directional_speed.x = (direction.x / info.s.w) * seq.moving_speed;
            seq.directional_speed.y = (direction.y / info.s.h) * seq.moving_speed;
            continue;
        }
        // else, we're just moving on to the next dest
        auto x = seq.directional_speed.x;
        auto y = seq.directional_speed.y;
        pos.x = pos.x + x;
        pos.y = pos.y + y;
    }
}

void CombatStatePlayerMovement::draw()
{
    using namespace Component;
    // LOG_FUNC

    auto tv = m_game->get_tv();

    for (auto &&[ent, node, pos, size] :
         m_registry->get()
             .view<Component::Combat::Node, Component::Pos, Component::Size>()
             .each()) {
        if (node.is_visited) {
            tv->FillRect(pos.x + 4, pos.y + 4, size.w - 4, size.h - 4,
                         olc::VERY_DARK_RED);
        }
        if (node.weight > 0) {
            tv->FillRect(pos.x + 4, pos.y + 4, size.w - 4, size.h - 4, olc::DARK_MAGENTA);
        }
        if (node.weight > 30) {
            tv->FillRect(pos.x + 4, pos.y + 4, size.w - 4, size.h - 4, olc::MAGENTA);
        }
        if (node.is_obstacle) {
            tv->FillRect(pos.x + 4, pos.y + 4, size.w - 4, size.h - 4, olc::WHITE);
        }
        std::stringstream ss;
        ss << m_registry->entity_name(ent);
        tv->DrawString(pos.x + 10, pos.y + size.h / 2, ss.str(), olc::WHITE, {0.5, 0.5});
    }

    for (auto &&[ent, pos, size] :
         m_registry->get()
             .view<Component::Combat::_Node_Start, Component::Pos, Component::Size>()
             .each()) {
        tv->FillRect(pos.x + 2, pos.y + 2, size.w - 2, size.h - 2, olc::BLUE);
    }
    // draw line for neighbour connections
    for (auto &&[ent, node, pos, size] :
         m_registry->get()
             .view<Component::Combat::Node, Component::Pos, Component::Size>()
             .each()) {
        for (auto i = 0; i < node.neighbour_count; ++i) {
            auto neighbour = node.neighbours[i];
            auto npos = m_registry->get_component<Component::Pos>(neighbour);
            auto nsize = m_registry->get_component<Component::Size>(neighbour);

            tv->DrawLine(pos.x + size.w / 2, pos.y + size.h / 2, npos->x + nsize->w / 2,
                         npos->y + nsize->h / 2, olc::WHITE, 0x0f0f0f0f);
        }
    }

    // draw path
    for (auto &&[ent, node, pos, size] :
         m_registry->get()
             .view<Component::Combat::_Node_End, Component::Combat::Node, Component::Pos,
                   Component::Size>()
             .each()) {
        tv->FillRect(pos.x + 2, pos.y + 2, size.w - 2, size.h - 2, olc::GREEN);

        // HACK this should be moved outside obvs
        auto get_parent = [this](entt::entity e) {
            return m_registry->get_component<Component::Combat::Node>(e)->parent;
        };
        auto curr = ent;
        while (true) {
            auto parent = get_parent(curr);
            if (parent == entt::null) {
                break;
            }
            auto cpos = m_registry->get_component<Component::Pos>(curr);
            auto csize = m_registry->get_component<Component::Size>(curr);
            auto ppos = m_registry->get_component<Component::Pos>(parent);
            auto psize = m_registry->get_component<Component::Size>(parent);
            tv->DrawLineDecal(olc::vf2d(cpos->x + csize->w / 2, cpos->y + csize->h / 2),
                              olc::vf2d(ppos->x + psize->w / 2, ppos->y + psize->h / 2),
                              olc::YELLOW);
            curr = parent;
        };
    }
}

bool CombatStatePlayerMovement::mouse_button_moved(Engine::MouseMovedEvent &event)
{
    auto get_name = []() { return "CombatSelect - mouse_button_moved()"; };

    return false;
}
bool CombatStatePlayerMovement::mouse_button_released(Engine::MouseButtonReleasedEvent &event)
{
    auto get_name = []() { return "CombatSelect - mouse_button_released()"; };

    if (event.get_button() == Engine::MouseButtonEvent::MouseButton::RightMouseButton) {
        return false;
    }
    for (auto &&[ent, hoverable, selectable] :
         m_registry->get()
             .view<Component::Interaction::Hoverable,
                   Component::Interaction::Selectable>()
             .each()) {
        if (hoverable.is_hovered) {
            Error("Release for button " << m_registry->entity_name(ent));
            switch (event.get_button()) {
            case Engine::MouseButtonEvent::MouseButton::LeftMouseButton: {
                if (selectable.on_left_select)
                    return selectable.on_left_select(m_registry, ent);
            } break;
            case Engine::MouseButtonEvent::MouseButton::RightMouseButton: {
                if (selectable.on_left_select)
                    return selectable.on_right_select(m_registry, ent);
            } break;
            case Engine::MouseButtonEvent::MouseButton::MiddleMouseButton: {
                if (selectable.on_left_select)
                    return selectable.on_middle_select(m_registry, ent);
            } break;
            }
        }
    }
    return false;
}
bool CombatStatePlayerMovement::mouse_button_pressed(Engine::MouseButtonPressedEvent &event)
{
    auto get_name = []() { return "CombatSelect - mouse_button_pressed()"; };
    // HACK
    Debug("Mouse " << event);

    return false;
}

}; // namespace PlayingState
