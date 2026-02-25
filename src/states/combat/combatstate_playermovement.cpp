#include "combatstate_playermovement.hpp"
#include "logger.hpp"
#include "utils/geometry.hpp"

namespace PlayingState
{

CombatStatePlayerMovement::CombatStatePlayerMovement(TacticalGame *ge) : CombatState(ge) {
    // TODO can use this for mapping buttons ig
    // if we're using buttons ?
    handler = std::make_unique<InputHandler>();
    m_controlled_entity = entt::null;
    m_tile_dest = entt::null;
}

void CombatStatePlayerMovement::enter(TacticalGame *ge) {
    auto reg = ge->registry();
    for (auto &&[ent] :
         reg->get().view<Component::Combat::Interaction::_Playable>().each()) {
        m_controlled_entity = ent;
    }

    for (auto &&[ent, holding] :
         reg->get().view<Component::Combat::CurrentlyHolding>().each()) {
        // HACK testing
        if (holding.value == m_controlled_entity) {
            reg->add_tag<Component::Combat::_Node_Start>(ent);
        }
    }

    Info("Currently controlling entity " << reg->entity_name(m_controlled_entity));
}

void CombatStatePlayerMovement::handle_input(TacticalGame *ge, Engine::Event &event)
{
    Engine::EventDispatcher dispatcher(ge, event);
    
    dispatcher.Dispatch<Engine::MouseButtonPressedEvent>(
        [this](TacticalGame *ge, Engine::MouseButtonPressedEvent &e) {
            return mouse_button_pressed(ge, e);
        });
    dispatcher.Dispatch<Engine::MouseButtonReleasedEvent>(
        [this](TacticalGame *ge, Engine::MouseButtonReleasedEvent &e) {
            return mouse_button_released(ge, e);
        });
    dispatcher.Dispatch<Engine::MouseMovedEvent>(
        [this](TacticalGame *ge, Engine::MouseMovedEvent &e) {
            return mouse_button_moved(ge, e);
        });
}

void CombatStatePlayerMovement::update(TacticalGame *ge)
{
    // move entity to selected spot, one line at a time
    using namespace Component;

    auto reg = ge->registry();
    for (auto &&[ent, pos, seq] :
         reg->get().view<Component::Pos, Combat::Moving>().each()) {
        // Debug("Currently on pos " << pos);
        // if we've reached next step in the sequence
        if (pos == seq.dest_sequence[seq.sequence_step]) {
            // have we arrived
            if (seq.sequence_step == 0) {
                seq.on_reached_dest(reg);
                continue;;
            }
            // else, incremnt and find new speed
            seq.sequence_step--;
            auto direction = seq.dest_sequence[seq.sequence_step+1] -
                             seq.dest_sequence[seq.sequence_step];

            direction.x = std::abs(direction.x);
            direction.y = std::abs(direction.y);
            seq.directional_speed.x = (direction.x / rect_w) * seq.moving_speed;
            seq.directional_speed.y = (direction.y / rect_h) * seq.moving_speed;
            Debug("Dir " << direction << " speed " << seq.directional_speed);
            continue;
        }
        // else, we're just moving on to the next dest
        auto x = seq.directional_speed.x;
        auto y = seq.directional_speed.y;
        Debug("Moving speed " << x << ", " << y);
        pos.x = pos.x + x;
        pos.y = pos.y + y;
    }
}

void CombatStatePlayerMovement::draw(TacticalGame *ge)
{
    using namespace Component;
    // LOG_FUNC
    auto reg = ge->registry();

    // HACK debugging purposes
    // auto sw = ge->ScreenWidth();
    // auto sh = ge->ScreenHeight();
    // auto w = (0.04);
    // auto h = (0.05);
    // auto rect_w = sw * w;
    // auto rect_h = sh * h;
    auto tv = ge->get_tv();

    for (auto &&[ent, node, pos, size] :
         reg->get()
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
        ss << reg->entity_name(ent);
        tv->DrawString(pos.x + 10, pos.y + size.h / 2, ss.str(), olc::WHITE, {0.5, 0.5});
    }

    for (auto &&[ent, pos, size] :
         reg->get()
             .view<Component::Combat::_Node_Start, Component::Pos, Component::Size>()
             .each()) {
        tv->FillRect(pos.x + 2, pos.y + 2, size.w - 2, size.h - 2, olc::BLUE);
    }
    // draw line for neighbour connections
    for (auto &&[ent, node, pos, size] :
         reg->get()
             .view<Component::Combat::Node, Component::Pos, Component::Size>()
             .each()) {
        for (auto i = 0; i < node.neighbour_count; ++i) {
            auto neighbour = node.neighbours[i];
            auto npos = reg->get_component<Component::Pos>(neighbour);
            auto nsize = reg->get_component<Component::Size>(neighbour);

            tv->DrawLine(pos.x + size.w / 2, pos.y + size.h / 2, npos->x + nsize->w / 2,
                         npos->y + nsize->h / 2, olc::WHITE, 0x0f0f0f0f);
        }
    }

    // draw path
    for (auto &&[ent, node, pos, size] :
         reg->get()
             .view<Component::Combat::_Node_End, Component::Combat::Node, Component::Pos,
                   Component::Size>()
             .each()) {
        tv->FillRect(pos.x + 2, pos.y + 2, size.w - 2, size.h - 2, olc::GREEN);

        // HACK this should be moved outside obvs
        auto get_parent = [reg](entt::entity e) {
            return reg->get_component<Component::Combat::Node>(e)->parent;
        };
        auto curr = ent;
        while (true) {
            auto parent = get_parent(curr);
            if (parent == entt::null) {
                break;
            }
            auto cpos = reg->get_component<Component::Pos>(curr);
            auto csize = reg->get_component<Component::Size>(curr);
            auto ppos = reg->get_component<Component::Pos>(parent);
            auto psize = reg->get_component<Component::Size>(parent);
            tv->DrawLineDecal(olc::vf2d(cpos->x + csize->w / 2, cpos->y + csize->h / 2),
                              olc::vf2d(ppos->x + psize->w / 2, ppos->y + psize->h / 2),
                              olc::YELLOW);
            curr = parent;
        };
    }
}

bool CombatStatePlayerMovement::mouse_button_moved(TacticalGame *ge, Engine::MouseMovedEvent &event)
{
    auto get_name = []() { return "CombatSelect - mouse_button_moved()"; };
    auto reg = ge->registry();
    auto tv = ge->get_tv();
    auto pos_mouse = ge->GetMousePos();

    // this should be moved to a separate system
    auto mouse_pos = tv->ScaleToWorld(pos_mouse) + tv->GetWorldOffset();
    for (auto &&[ent, pos, size, hoverable] :
         reg->get()
             .view<Component::Pos, Component::Size, Interaction::Hoverable>()
             .each()) {
        hoverable.is_hovered = is_point_inside_rect(pos, size, mouse_pos);
        if (hoverable.is_hovered && ent != m_tile_dest) {
            auto curr_end = reg->get_single_entity_with_tag<Component::Combat::_Node_End>();
            reg->remove_component<Component::Combat::_Node_End>(m_tile_dest);
            m_tile_dest = ent;
            reg->add_tag<Component::Combat::_Node_End>(m_tile_dest);

            solve_a_star(reg);
        }
    }

    // if (ge->animation_tick())
    //     solve_a_star(ge->registry());

    return false;
}
bool CombatStatePlayerMovement::mouse_button_released(TacticalGame *ge, Engine::MouseButtonReleasedEvent &event)
{
    auto get_name = []() { return "CombatSelect - mouse_button_released()"; };
    Debug("Released");
    using namespace Component;
    auto reg = ge->registry();
    
    Component::Combat::Moving moving_sequence;
    auto pos = reg->get_component<Pos>(m_controlled_entity);
    
    entt::entity start_tile_entity = reg->get_entity_on_check<Combat::CurrentlyHolding>([this](Combat::CurrentlyHolding c) {
        return m_controlled_entity==c.value;
    });

    Debug("Starting at " << reg->entity_name(start_tile_entity));
    
    moving_sequence.moving_speed = 4;
    moving_sequence.directional_speed = {0,0};
    auto traversing_entity = m_tile_dest;
    while(traversing_entity!=entt::null) {
        moving_sequence.dest_sequence.push_back(reg->unsafe_get_component<Pos>(traversing_entity));
        traversing_entity = reg->unsafe_get_component<Combat::Node>(traversing_entity).parent;
    }

    moving_sequence.sequence_step = moving_sequence.dest_sequence.size()-1;
    for(auto step : moving_sequence.dest_sequence) {
        Debug("Seq step: " << step);
    }
    moving_sequence.on_reached_dest = [this, start_tile_entity](GameRegistry *reg) {
        auto get_name = [&, start_tile_entity]() { return "On Dest Reached !"; };
        Info("Reached dest ! Removing from " << start_tile_entity);
        reg->unsafe_remove_component<Combat::CurrentlyHolding>(start_tile_entity);
        reg->add_component<Combat::CurrentlyHolding>(m_tile_dest, m_controlled_entity);
        
        reg->unsafe_remove_component<Combat::Moving>(m_controlled_entity);

        // HACK
        // switch to next state ?
    };
    reg->unsafe_add_component<Combat::Moving>(m_controlled_entity, moving_sequence);
    return true;
}
bool CombatStatePlayerMovement::mouse_button_pressed(TacticalGame *ge, Engine::MouseButtonPressedEvent &event)
{
    auto get_name = []() { return "CombatSelect - mouse_button_pressed()"; };
    // HACK
    Debug("Mouse " << event);
    // auto screen_w = ge->ScreenWidth();
    // auto sh = ge->ScreenHeight();
    // auto w = (0.04);
    // auto h = (0.05);
    // auto rect_w = screen_w * w;
    // auto rect_h = sh * h;
    auto offs_x = (screen_w / 2) - (rect_w * tile_amt_x / 2);
    auto offs_y = (screen_h / 2) - (rect_h * tile_amt_y / 2);
    auto reg = ge->registry();
    // TODO this logic can be remade
    for (auto [ent, pos, size, selectable, hoverable, node] :
         reg->get().view<Component::Pos, Component::Size, Interaction::Selectable,
         Interaction::Hoverable, Component::Combat::Node>()
             .each()) {
        
        if (event.get_button() == Engine::MouseButtonEvent::MouseButton::LeftMouseButton) {
            if (hoverable.is_hovered) {
                
                return true;
            }
        }
        if (event.get_button() == Engine::MouseButtonEvent::MouseButton::RightMouseButton) {
            if (hoverable.is_hovered) {
                auto x_normalized = ((pos.x - offs_x) / rect_w);
                auto y_normalized = ((pos.y - offs_y) / rect_h);
                auto index_pos = x_normalized + (tile_amt_x * y_normalized);
                if (selectable.is_selected) {
                    selectable.is_selected = false;
                    node.is_obstacle = false;
                } else {
                    selectable.is_selected = true;
                    node.is_obstacle = true;
                }
                return true;
            }
        }
    }
    return false;
    }






    // HACK for testing
void CombatStatePlayerAction::update(TacticalGame *ge)
{
    // move entity to selected spot, one line at a time
    using namespace Component;

    auto reg = ge->registry();
}


void CombatStatePlayerAction::draw(TacticalGame *ge)
{
    using namespace Component;
    // LOG_FUNC
    auto reg = ge->registry();


    ge->DrawStringDecal({100, 100}, "IN PLAYER ACTION STATE", olc::WHITE, {1,4});
}

    

}; // namespace PlayingState
