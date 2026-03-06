#include "combatstate.hpp"

#include "logger.hpp"

#include "utils/ecs.hpp"
#include "utils/geometry.hpp"
#include "utils/complex_datatypes.hpp"

#include "asset_manager.hpp"

using namespace PlayingState;

class TestInput : public Input
{
  public:
    std::string get_name() const final { return "Test Input StarState key P"; };
    void execute(TacticalGame *ge) final
    {
        
    }
};

class PanInputStart : public Input
{
  public:
    std::string get_name() const final { return "InputPanningStart"; };
    void execute(TacticalGame *ge) final
    {
        auto tv = ge->get_tv();
        auto pos_mouse = ge->GetMousePos();
        ge->get_tv()->StartPan(pos_mouse);
    }
};

class ScrollUpInput : public Input
{
  public:
    std::string get_name() const final { return "ScrollUpInputStart"; };
    void execute(TacticalGame *ge) final
    {
        auto tv = ge->get_tv();
        auto pos_mouse = ge->GetMousePos();
        tv->ZoomAtScreenPos(0.5f, pos_mouse);
    }
};

class ScrollDownInput : public Input
{
  public:
    std::string get_name() const final { return "ScrollDownInputStart"; };
    void execute(TacticalGame *ge) final
    {
        auto tv = ge->get_tv();
        auto pos_mouse = ge->GetMousePos();
        tv->ZoomAtScreenPos(2.f, pos_mouse);
    }
};

class PanInputEnd : public Input
{
  public:
    std::string get_name() const final { return "InputPanningEnd"; };
    void execute(TacticalGame *ge) final
    {
        auto tv = ge->get_tv();
        auto pos_mouse = ge->GetMousePos();
        ge->get_tv()->EndPan(pos_mouse);
    }
};

class PanInputUpdate : public Input
{
  public:
    std::string get_name() const final { return "InputPanningUpdate"; };
    void execute(TacticalGame *ge) final
    {
        auto tv = ge->get_tv();
        auto pos_mouse = ge->GetMousePos();
        ge->get_tv()->UpdatePan(pos_mouse);
    }
};

bool CombatState::mouse_button_released(Engine::MouseButtonReleasedEvent &event)
{
    auto get_name = []() { return "Combat - mouse_button_released()"; };
    if (event.get_button() == Engine::MouseButtonEvent::MouseButton::MiddleMouseButton) {
        auto &camera = m_registry->unsafe_get_world_component<Component::World::Camera>();
        camera.is_panning = false;
        return true;
    }

    return false;
}
bool CombatState::mouse_button_pressed(Engine::MouseButtonPressedEvent &event)
{
    auto get_name = []() { return "Combat - mouse_button_pressed()"; };
    if (event.get_button() == Engine::MouseButtonEvent::MouseButton::MiddleMouseButton) {
        auto &camera = m_registry->unsafe_get_world_component<Component::World::Camera>();
        camera.is_panning = true;
        return true;
    }
    return false;
}
// bool CombatState::mouse_button_pressed(GameEvent &event)
// {
//     auto get_name = []() { return "Combat - mouse_button_pressed()"; };
//     if (event.get_button() == Engine::MouseButtonEvent::MouseButton::MiddleMouseButton) {
//         auto tv = ge->get_tv();
//         auto pos_mouse = ge->GetMousePos();
//         ge->get_tv()->StartPan(pos_mouse);

//         is_panning = true;
//         return true;
//     }
//     return false;
// }
void CombatState::pause() { LOG_FUNC }
void CombatState::resume() { LOG_FUNC }

void CombatState::handle_input(Engine::Event &event)
{
    // LOG_FUNC
    //  HACK FOR PANNING
    Engine::EventDispatcher dispatcher(event);
    dispatcher.Dispatch<Engine::MouseButtonPressedEvent>(
        [this](Engine::MouseButtonPressedEvent &e) {
            return mouse_button_pressed(e);
        });
    dispatcher.Dispatch<Engine::MouseButtonReleasedEvent>(
        [this](Engine::MouseButtonReleasedEvent &e) {
            return mouse_button_released(e);
        });
}

void CombatState::update()
{

}

void CombatState::draw()
{
    // HACK
    if(m_game->GetMouseWheel()>0) {
        m_game->get_tv()->ZoomAtScreenPos(2, m_game->GetMousePos());
    }
    if(m_game->GetMouseWheel()<0) {
        m_game->get_tv()->ZoomAtScreenPos(0.5, m_game->GetMousePos());
    }
    
}

// TODO move to separate thread
void CombatState::solve_a_star()
{
    auto get_node_pos = [this](entt::entity e) { return m_registry->entity_name(e); };

    entt::entity ent_start = entt::null;
    entt::entity ent_end = entt::null;
    entt::entity ent_current = entt::null;
    for (auto [ent] : m_registry->get().view<Component::Combat::_Node_Start>().each()) {
        ent_start = ent;
        ent_current = ent;
    }
    for (auto [ent] : m_registry->get().view<Component::Combat::_Node_End>().each()) {
        ent_end = ent;
    }
    if (ent_start == entt::null || ent_end == entt::null) {
        return;
    }
    Trace(
        "Solving from "
        << get_node_pos(ent_start) << " to "
        << get_node_pos(ent_end)) for (auto &&[ent, node] :
                                       m_registry->get().view<Component::Combat::Node>().each())
    {
        node.is_visited = false;
        node.global_goal = INFINITY;
        node.local_goal = INFINITY;
        node.parent = entt::null;
    }

    auto get_node = [this](entt::entity a) {
        return m_registry->get_component<Component::Combat::Node>(a);
    };

    auto distance = [this](entt::entity enta, entt::entity entb) {
        auto a = *m_registry->get_component<Component::Pos>(enta);
        auto b = *m_registry->get_component<Component::Pos>(entb);
        return sqrtf((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
    };

    auto heuristic = [distance](entt::entity a, entt::entity b) {
        return distance(a, b); // + (a.weight + b.weight) * (a.weight + b.weight);
    };

    auto node_start = get_node(ent_start);
    node_start->local_goal = 0.f;
    node_start->global_goal = heuristic(ent_start, ent_end);
    // list for sort func
    std::list<entt::entity> not_tested_nodes;
    not_tested_nodes.push_back(ent_start);
    // while (!not_tested_nodes.empty())
    // {
    while (!not_tested_nodes.empty() && ent_current != ent_end) {
        not_tested_nodes.sort([get_node](entt::entity ent_a, entt::entity ent_b) {
            auto lhs = get_node(ent_a);
            auto rhs = get_node(ent_b);
            return lhs->global_goal < rhs->global_goal;
        });

        while (!not_tested_nodes.empty() &&
               get_node(not_tested_nodes.front())->is_visited)
            not_tested_nodes.pop_front();

        if (not_tested_nodes.empty())
            break;

        ent_current = not_tested_nodes.front();
        auto node_current = m_registry->get_component<Component::Combat::Node>(ent_current);
        node_current->is_visited = true;
        for (auto i = 0; i < node_current->neighbour_count; ++i) {
            auto ent_neighbour = node_current->neighbours[i];
            auto neighbour = m_registry->get_component<Component::Combat::Node>(ent_neighbour);
            if (!neighbour->is_visited && !neighbour->is_obstacle)
                not_tested_nodes.push_back(ent_neighbour);
            float possible_local_goal =
                node_current->local_goal + distance(ent_current, ent_neighbour);
            if (possible_local_goal < neighbour->local_goal) {
                neighbour->parent = ent_current;
                neighbour->local_goal = possible_local_goal;
                neighbour->global_goal =
                    neighbour->local_goal + heuristic(ent_neighbour, ent_end);
            }
        }
    }
}
