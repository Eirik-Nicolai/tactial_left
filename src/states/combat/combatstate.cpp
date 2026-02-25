#include "combatstate.hpp"

#include "logger.hpp"

#include "utils/ecs.hpp"
#include "utils/geometry.hpp"
#include "utils/complex_datatypes.hpp"

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

bool CombatState::mouse_button_released(TacticalGame *ge, Engine::MouseButtonReleasedEvent &event)
{
    auto get_name = []() { return "Combat - mouse_button_released()"; };
    if (event.get_button() == Engine::MouseButtonEvent::MouseButton::MiddleMouseButton) {
        auto tv = ge->get_tv();
        auto pos_mouse = ge->GetMousePos();
        ge->get_tv()->EndPan(pos_mouse);

        is_panning = false;
        return true;
    }

    return false;
}
bool CombatState::mouse_button_pressed(TacticalGame *ge, Engine::MouseButtonPressedEvent &event)
{
    auto get_name = []() { return "Combat - mouse_button_pressed()"; };
    if (event.get_button() == Engine::MouseButtonEvent::MouseButton::MiddleMouseButton) {
        auto tv = ge->get_tv();
        auto pos_mouse = ge->GetMousePos();
        ge->get_tv()->StartPan(pos_mouse);

        is_panning = true;
        return true;
    }
    return false;
}
// bool CombatState::mouse_button_pressed(TacticalGame *ge, GameEvent &event)
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


CombatState::CombatState(TacticalGame* ge)
{
    LOG_FUNC
    handler = std::make_unique<InputHandler>();

    screen_w = ge->ScreenWidth();
    screen_h = ge->ScreenHeight();
    auto w = (0.04);
    auto h = (0.05);
    rect_w = screen_w * w;
    rect_h = screen_h * h;
    
}
CombatState::~CombatState() { LOG_FUNC }

void CombatState::pause(TacticalGame *ge) { LOG_FUNC }
void CombatState::resume(TacticalGame *ge) { LOG_FUNC }

void CombatState::enter(TacticalGame *ge)
{
    LOG_FUNC

    Debug("Entering combat state");
    auto reg = ge->registry();
    auto tv = ge->get_tv();

    using namespace Component;

    auto offs_x = (screen_w / 2) - (rect_w * tile_amt_x / 2);
    auto offs_y = (screen_h / 2) - (rect_h * tile_amt_y / 2);

    // convenience temp storage for keeping track of neighbours
    int combat_tile_index = 0;
    std::array<entt::entity, tile_amt_x*tile_amt_y> combat_tiles;
    for (auto x = 0; x < tile_amt_x; x++)
        for (auto y = 0; y < tile_amt_y; y++) {
            std::stringstream ss;
            ss << "("<<x<<"," << y <<")";
            entt::entity tile = reg->create_entity(ss.str().c_str());
            reg->add_component<Interaction::Selectable>(tile, false);
            reg->add_component<Interaction::Hoverable>(tile, false);

            Pos p;
            p.x = offs_x + (x * rect_w);
            p.y = offs_y + (y * rect_h);
            reg->add_component<Pos>(tile, p);
            reg->add_component<Size>(tile, (float)rect_w, (float)rect_h);
            //combat_tiles[x + (tile_amt_x * y)] = std::make_shared<Node>(p.x, p.y);
            Combat::Node node;
            node.is_visited = false;
            node.is_obstacle = false;
            node.neighbour_count = 0;
            node.weight = 0;
            node.parent = entt::null;
            reg->add_component<Combat::Node>(tile,node);
            Rendering::Wireframe wire;
            wire.color = olc::DARK_RED;
            wire.type = Rendering::Wireframe::TYPE::SQUARE;
            reg->add_component<Rendering::Wireframe>(tile, wire);
            combat_tiles[x + (tile_amt_x * y)] = tile;
        }

    for (auto x = 0; x < tile_amt_x; x++)
        for (auto y = 0; y < tile_amt_y; y++) {
            auto curr_node = combat_tiles[x + (tile_amt_x * y)];
            if (y > 0) {
                reg->get_component<Combat::Node>(curr_node)->add_neighbour(
                    combat_tiles[x + 0 + (tile_amt_x * (y - 1))]);
            }
            if (y < tile_amt_y - 1) {
                reg->get_component<Combat::Node>(curr_node)->add_neighbour(
                    combat_tiles[x + 0 + (tile_amt_x * (y + 1))]);
            }
            if (x > 0) {
                reg->get_component<Combat::Node>(curr_node)->add_neighbour(
                    combat_tiles[x - 1 + (tile_amt_x * (y - 0))]);
            }
            if (x < tile_amt_x - 1) {
                reg->get_component<Combat::Node>(curr_node)->add_neighbour(
                    combat_tiles[x + 1 + (tile_amt_x * (y + 0))]);
            }
        }

    // testing
    // reg->add_tag<Combat::_Node_Start>(combat_tiles[0]);
    // reg->add_tag<Combat::_Node_End>(combat_tiles[5]);

    auto path_1 = "assets/Cute_Fantasy_Free/Player/Player.png";
    auto player = reg->create_entity("Player");
    auto player_decal_index = ge->load_decal(path_1, false, true);
    if(!player_decal_index){
        Error("Loading decal returned " << player_decal_index.error());
        throw std::runtime_error("");
    }
    
    Debug("LOADED SPRITE " << player_decal_index.value());

    reg->add_component<Pos>(player, 50.f, 50.f);
    reg->add_component<Size>(player, 300.f, 300.f);
   
    Debug("creating rendering manager");
    reg->add_component<Rendering::Spritesheet>(player, player_decal_index.value(), Size(32,32));

    Rendering::RenderingManager rendering_manager;
    // rendering_manager.sprite_sheet = player;
    rendering_manager.pos_sprite_sheet = {0.f, 0.f};
    rendering_manager.sprite_scale = {1.f, 1.f};
    reg->add_component<Rendering::RenderingManager>(player, rendering_manager);

    reg->add_tag<Rendering::Layer::_closest>(player);

    Debug("creating anim idle");
    Animation::SpriteSheetAnimation idle_animation;
    idle_animation.animation_name = "idle";
    idle_animation.frames[0] = Animation::AnimationFrame{Pos{0,0}, 5};
    idle_animation.frames[1] = Animation::AnimationFrame{Pos{1,0}, 5};
    idle_animation.frames[2] = Animation::AnimationFrame{Pos{2,0}, 5};
    idle_animation.frames[3] = Animation::AnimationFrame{Pos{3,0}, 5};
    idle_animation.frames[4] = Animation::AnimationFrame{Pos{4,0}, 5};
    idle_animation.frames[5] = Animation::AnimationFrame{Pos{5,0}, 20};
    idle_animation.is_looping = true;
    idle_animation.frame_animation_length = 6;

    Debug("creating anim walking");
    Animation::SpriteSheetAnimation walking_east;
    walking_east.animation_name = "walking_east";
    walking_east.frames[0] = Animation::AnimationFrame{Pos{0,4}, 4};
    walking_east.frames[1] = Animation::AnimationFrame{Pos{1,4}, 4};
    walking_east.frames[2] = Animation::AnimationFrame{Pos{2,4}, 4};
    walking_east.frames[3] = Animation::AnimationFrame{Pos{3,4}, 4};
    walking_east.frames[4] = Animation::AnimationFrame{Pos{4,4}, 4};
    walking_east.frames[5] = Animation::AnimationFrame{Pos{5,4}, 4};
    walking_east.is_looping = true;
    walking_east.frame_animation_length = 6;

    Debug("creating anim dead");
    Animation::SpriteSheetAnimation dead;
    dead.animation_name = "dead";
    dead.frames[0] = Animation::AnimationFrame{Pos{0,9}, 10};
    dead.frames[1] = Animation::AnimationFrame{Pos{1,9}, 10};
    dead.frames[2] = Animation::AnimationFrame{Pos{2,9}, 10};
    dead.frames[3] = Animation::AnimationFrame{Pos{3,9}, 10};
    dead.is_looping = false;
    dead.frame_animation_length = 4;

    Debug("Created sprite sheet with animation " << idle_animation.animation_name);
    Animation::AnimationList list;
    list.animations[list.animations_amt++] = idle_animation;
    list.animations[list.animations_amt++] = walking_east;
    list.animations[list.animations_amt++] = dead;
    reg->add_component<Animation::AnimationList>(player, list);

    Debug("creating anim manager");
    Animation::AnimManager mng;
    mng.curr_animation = idle_animation;
    mng.index_curren_animation = 0;
    mng.index_curren_frame = 0;
    mng.frames_elapsed = 0;
    reg->add_component<Animation::AnimManager>(player, mng);

    reg->add_tag<Component::Combat::Interaction::_Playable>(player);
    reg->add_component<Component::Combat::CurrentlyHolding>(combat_tiles[0], player);
    reg->unsafe_set_component<Component::Pos>(player, [&](Component::Pos &p){
        p = (*reg->get_component<Component::Pos>(combat_tiles[0]));
    });
}

void CombatState::handle_input(TacticalGame *ge, Engine::Event &event)
{
    // LOG_FUNC
    //  HACK FOR PANNING
    Engine::EventDispatcher dispatcher(ge, event);
    dispatcher.Dispatch<Engine::MouseButtonPressedEvent>(
        [this](TacticalGame *ge, Engine::MouseButtonPressedEvent &e) {
            return mouse_button_pressed(ge, e);
        });
    dispatcher.Dispatch<Engine::MouseButtonReleasedEvent>(
        [this](TacticalGame *ge, Engine::MouseButtonReleasedEvent &e) {
            return mouse_button_released(ge, e);
        });
}

void CombatState::update(TacticalGame *ge)
{
    
    if(ge->GetMouseWheel()>0) {
        ge->get_tv()->ZoomAtScreenPos(2, ge->GetMousePos());
    }
    if(ge->GetMouseWheel()<0) {
        ge->get_tv()->ZoomAtScreenPos(0.5, ge->GetMousePos());
    }
    
    if (is_panning) {
        auto pos_mouse = ge->GetMousePos();
        ge->get_tv()->UpdatePan(pos_mouse);
    }
}

void CombatState::draw(TacticalGame *ge)
{

}

// TODO move to separate thread
void CombatState::solve_a_star(GameRegistry* reg)
{
    auto get_node_pos = [reg](entt::entity e) { return reg->entity_name(e); };

    entt::entity ent_start = entt::null;
    entt::entity ent_end = entt::null;
    entt::entity ent_current = entt::null;
    for (auto [ent] : reg->get().view<Component::Combat::_Node_Start>().each()) {
        ent_start = ent;
        ent_current = ent;
    }
    for (auto [ent] : reg->get().view<Component::Combat::_Node_End>().each()) {
        ent_end = ent;
    }
    if (ent_start == entt::null || ent_end == entt::null) {
        return;
    }
    Trace(
        "Solving from "
        << get_node_pos(ent_start) << " to "
        << get_node_pos(ent_end)) for (auto &&[ent, node] :
                                       reg->get().view<Component::Combat::Node>().each())
    {
        node.is_visited = false;
        node.global_goal = INFINITY;
        node.local_goal = INFINITY;
        node.parent = entt::null;
    }

    auto get_node = [reg](entt::entity a) {
        return reg->get_component<Component::Combat::Node>(a);
    };

    auto distance = [reg](entt::entity enta, entt::entity entb) {
        auto a = *reg->get_component<Component::Pos>(enta);
        auto b = *reg->get_component<Component::Pos>(entb);
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
        auto node_current = reg->get_component<Component::Combat::Node>(ent_current);
        node_current->is_visited = true;
        for (auto i = 0; i < node_current->neighbour_count; ++i) {
            auto ent_neighbour = node_current->neighbours[i];
            auto neighbour = reg->get_component<Component::Combat::Node>(ent_neighbour);
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
