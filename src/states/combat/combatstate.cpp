#include "combatstate.hpp"

#include "logger.hpp"

#include "utils/ecs.hpp"
#include "utils/geometry.hpp"
#include "utils/complex_datatypes.hpp"

#include "components/components.hpp"
#include "components/animation.hpp"
#include "components/interaction.hpp"
#include "components/rendering.hpp"

using namespace PlayingState;

class TestInput : public Input
{
  public:
    std::string get_name() const final { return "Test Input StarState key P"; };
    void execute(TacticalGame *ge) final
    {
        // Error("Triggered input {}", get_name());
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

bool CombatState::mouse_button_released(TacticalGame *ge, MouseButtonReleasedEvent &event)
{
    auto get_name = []() { return "Combat - mouse_button_released()"; };
    if (event.get_button() == MouseButtonEvent::MouseButton::MiddleMouseButton) {
        auto tv = ge->get_tv();
        auto pos_mouse = ge->GetMousePos();
        ge->get_tv()->EndPan(pos_mouse);

        is_panning = false;
        return true;
    }

    return false;
}
bool CombatState::mouse_button_pressed(TacticalGame *ge, MouseButtonPressedEvent &event)
{
    auto get_name = []() { return "Combat - mouse_button_pressed()"; };
    if (event.get_button() == MouseButtonEvent::MouseButton::MiddleMouseButton) {
        auto tv = ge->get_tv();
        auto pos_mouse = ge->GetMousePos();
        ge->get_tv()->StartPan(pos_mouse);

        is_panning = true;
        return true;
    }
    Debug("Mouse " << event) auto sw = ge->ScreenWidth();
    auto sh = ge->ScreenHeight();
    auto w = (0.04);
    auto h = (0.05);
    auto rect_w = sw * w;
    auto rect_h = sh * h;
    auto offs_x = (sw / 2) - (rect_w * tile_amt_x / 2);
    auto offs_y = (sh / 2) - (rect_h * tile_amt_y / 2);
    auto reg = ge->registry();
    for (auto [ent, pos, size, selectable, hoverable] :
         reg->get().view<Pos, Size, Interaction::_selectable, Interaction::_hoverable>()
             .each()) {
        if (event.get_button() == MouseButtonEvent::MouseButton::LeftMouseButton) {
            if (hoverable.is_hovered) {
                auto x_normalized = ((pos.x - offs_x) / rect_w);
                auto y_normalized = ((pos.y - offs_y) / rect_h);
                auto index_pos = x_normalized + (tile_amt_x * y_normalized);
                if (selectable.is_selected) {
                    selectable.is_selected = false;
                    combat_tiles[index_pos]->is_obstacle = false;
                } else {
                    selectable.is_selected = true;
                    combat_tiles[index_pos]->is_obstacle = true;
                }
                return true;
            }
        }
        if (event.get_button() == MouseButtonEvent::MouseButton::RightMouseButton) {
            if (hoverable.is_hovered) {
                auto x_normalized = ((pos.x - offs_x) / rect_w);
                auto y_normalized = ((pos.y - offs_y) / rect_h);
                auto index_pos = x_normalized + (tile_amt_x * y_normalized);
                combat_tiles[index_pos]->is_obstacle = false;
                node_start = combat_tiles[index_pos];
                return true;
            }
        }
    }
    return true;
}

CombatState::CombatState()
{
    LOG_FUNC
    handler = std::make_unique<InputHandler>();
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

    auto sw = ge->ScreenWidth();
    auto sh = ge->ScreenHeight();
    auto w = (0.04);
    auto h = (0.05);
    auto rect_w = sw * w;
    auto rect_h = sh * h;
    auto offs_x = (sw / 2) - (rect_w * tile_amt_x / 2);
    auto offs_y = (sh / 2) - (rect_h * tile_amt_y / 2);

    Info("Node amt {}", combat_tiles.size());
    for (auto x = 0; x < tile_amt_x; x++)
        for (auto y = 0; y < tile_amt_y; y++) {

            auto tile = reg->create_entity("Combat tile");
            reg->add_component<Interaction::_selectable>(tile, false);
            reg->add_component<Interaction::_hoverable>(tile, false);
            reg->add_tag<Combat::Terrain::_path>(tile);

            Pos p;
            p.x = offs_x + (x * rect_w);
            p.y = offs_y + (y * rect_h);
            reg->add_component<Pos>(tile, p);
            reg->add_component<Size>(tile, (float)rect_w, (float)rect_h);
            combat_tiles[x + (tile_amt_x * y)] = std::make_shared<Node>(p.x, p.y);

            Rendering::Wireframe wire;
            wire.color = olc::DARK_RED;
            wire.type = Rendering::Wireframe::TYPE::SQUARE;
            reg->add_component<Rendering::Wireframe>(tile, wire);
        }

    for (auto x = 0; x < tile_amt_x; x++)
        for (auto y = 0; y < tile_amt_y; y++) {
            if (y > 0)
                combat_tiles[x + (tile_amt_x * y)]->add_neighbour(
                    combat_tiles[x + 0 + (tile_amt_x * (y - 1))]);
            if (y < tile_amt_y - 1)
                combat_tiles[x + (tile_amt_x * y)]->add_neighbour(
                    combat_tiles[x + 0 + (tile_amt_x * (y + 1))]);
            if (x > 0)
                combat_tiles[x + (tile_amt_x * y)]->add_neighbour(
                    combat_tiles[x - 1 + (tile_amt_x * (y - 0))]);
            if (x < tile_amt_x - 1)
                combat_tiles[x + (tile_amt_x * y)]->add_neighbour(
                    combat_tiles[x + 1 + (tile_amt_x * (y + 0))]);
        }

    node_start = combat_tiles[0];
    node_end = combat_tiles[5];

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
    reg->add_component<Interaction::_selectable>(player);
    reg->add_component<Interaction::_hoverable>(player);

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
    idle_animation.name = "idle";
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
    walking_east.name = "walking_east";
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
    dead.name = "dead";
    dead.frames[0] = Animation::AnimationFrame{Pos{0,9}, 10};
    dead.frames[1] = Animation::AnimationFrame{Pos{1,9}, 10};
    dead.frames[2] = Animation::AnimationFrame{Pos{2,9}, 10};
    dead.frames[3] = Animation::AnimationFrame{Pos{3,9}, 10};
    dead.is_looping = false;
    dead.frame_animation_length = 4;

    Debug("Created sprite sheet with animation {}", idle_animation.name);
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
    mng.name = "Player Anim Manager";
    mng.frames_elapsed = 0;
    reg->add_component<Animation::AnimManager>(player, mng);

}
void CombatState::exit(TacticalGame *ge) { LOG_FUNC }

void CombatState::handle_input(TacticalGame *ge, Event &event)
{
    // LOG_FUNC
    //  HACK FOR PANNING
    EventDispatcher dispatcher(ge, event);
    dispatcher.Dispatch<MouseButtonPressedEvent>(
        [this](TacticalGame *ge, MouseButtonPressedEvent &e) {
            return mouse_button_pressed(ge, e);
        });
    dispatcher.Dispatch<MouseButtonReleasedEvent>(
        [this](TacticalGame *ge, MouseButtonReleasedEvent &e) {
            return mouse_button_released(ge, e);
        });
}

void CombatState::update(TacticalGame *ge)
{
    // LOG_FUN
    auto reg = ge->registry();
    auto tv = ge->get_tv();
    auto pos_mouse = ge->GetMousePos();

    if (is_panning) {
        ge->get_tv()->UpdatePan(pos_mouse);
    }
    // this should be moved to a separate system
    auto mouse_pos = tv->ScaleToWorld(pos_mouse) + tv->GetWorldOffset();
    for (auto &&[ent, pos, size, hoverable] :
         reg->get().view<Pos, Size, Interaction::_hoverable>().each()) {
        hoverable.is_hovered = is_point_inside_rect(pos, size, mouse_pos);
    }

    if (ge->animation_tick())
        solve_a_star();
}

void CombatState::draw(TacticalGame *ge)
{
    // LOG_FUNC

    auto reg = ge->registry();
    for (auto &&[ent, wire, hoverable] :
         reg->get().view<Rendering::Wireframe, Interaction::_hoverable>().each()) {
        if (hoverable.is_hovered) {
            wire.color = olc::RED;
            wire.type = Rendering::Wireframe::TYPE::SQUARE;
        } else {
            wire.color = olc::DARK_RED;
            wire.type = Rendering::Wireframe::TYPE::SQUARE;
        }
    }
    for (auto &&[ent, wire, _] :
         reg->get().view<Rendering::Wireframe, Interaction::_selectable>().each()) {
        wire.color = olc::DARK_GREY;
        wire.type = Rendering::Wireframe::TYPE::SQUARE;
    }

    // HACK debugging purposes
    auto sw = ge->ScreenWidth();
    auto sh = ge->ScreenHeight();
    auto w = (0.04);
    auto h = (0.05);
    auto rect_w = sw * w;
    auto rect_h = sh * h;
    auto tv = ge->get_tv();

    for (auto x = 0; x < tile_amt_x; x++)
        for (auto y = 0; y < tile_amt_y; y++) {
            auto curr_node = combat_tiles[x + (tile_amt_x * y)];

            if (curr_node->is_visited) {
                tv->FillRect(curr_node->x + 4, curr_node->y + 4, rect_w - 4, rect_h - 4,
                             olc::VERY_DARK_RED);
            }
            if (curr_node->weight > 0) {
                tv->FillRect(curr_node->x + 4, curr_node->y + 4, rect_w - 4, rect_h - 4,
                             olc::DARK_MAGENTA);
            }
            if (curr_node->weight > 30) {
                tv->FillRect(curr_node->x + 4, curr_node->y + 4, rect_w - 4, rect_h - 4,
                             olc::MAGENTA);
            }
            if (curr_node->is_obstacle) {
                tv->FillRect(curr_node->x + 4, curr_node->y + 4, rect_w - 4, rect_h - 4,
                             olc::WHITE);
            }
        }

    if (node_start)
        tv->FillRect(node_start->x + 2, node_start->y + 2, rect_w - 2, rect_h - 2,
                     olc::BLUE);

    // draw line for neighbour connections
    for (auto x = 0; x < tile_amt_x; x++)
        for (auto y = 0; y < tile_amt_y; y++) {
            // Error("Drawing for {} {} ", x, y);
            auto curr_node = combat_tiles[x + (tile_amt_x * y)];
            for (auto i = 0; i < curr_node->neighbour_count; ++i) {
                auto neighbour = curr_node->neighbours[i];
                tv->DrawLine(curr_node->x + rect_w / 2, curr_node->y + rect_h / 2,
                             neighbour->x + rect_w / 2, neighbour->y + rect_h / 2,
                             olc::WHITE, 0x0f0f0f0f);
            }
        }

    // draw path
    if (node_end) {
        tv->FillRect(node_end->x + 2, node_end->y + 2, rect_w - 2, rect_h - 2,
                     olc::GREEN);

        auto n = node_end;
        while (n->parent) {
            tv->DrawLineDecal(
                olc::vf2d(n->x + rect_w / 2, n->y + rect_h / 2),
                olc::vf2d(n->parent->x + rect_w / 2, n->parent->y + rect_h / 2),
                olc::YELLOW);
            n = n->parent;
        }
    }
}

// TODO move to separate thread
void CombatState::solve_a_star()
{
    for (auto x = 0; x < tile_amt_x; x++)
        for (auto y = 0; y < tile_amt_y; y++) {
            auto index = x + (tile_amt_x * y);
            combat_tiles[index]->is_visited = false;
            combat_tiles[index]->global_goal = INFINITY;
            combat_tiles[index]->local_goal = INFINITY;
            combat_tiles[index]->parent = nullptr;
        }

    auto distance = [](std::shared_ptr<Node> a, std::shared_ptr<Node> b) {
        return sqrtf((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y));
    };

    auto heuristic = [distance](std::shared_ptr<Node> a, std::shared_ptr<Node> b) {
        return distance(a, b) + (a->weight + b->weight) * (a->weight + b->weight);
    };

    auto node_current = node_start;
    node_start->local_goal = 0.f;
    node_start->global_goal = heuristic(node_start, node_end);
    // list for sort func
    std::list<std::shared_ptr<Node>> not_tested_nodes;
    not_tested_nodes.push_back(node_start);
    while (!not_tested_nodes.empty())
    // while(!not_tested_nodes.empty() && node_current != node_end)
    {
        not_tested_nodes.sort([](std::shared_ptr<Node> lhs, std::shared_ptr<Node> rhs) {
            return lhs->global_goal < rhs->global_goal;
        });

        while (!not_tested_nodes.empty() && not_tested_nodes.front()->is_visited)
            not_tested_nodes.pop_front();

        if (not_tested_nodes.empty())
            break;

        node_current = not_tested_nodes.front();
        node_current->is_visited = true;
        for (auto i = 0; i < node_current->neighbour_count; ++i) {
            auto neighbour = node_current->neighbours[i];
            if (!neighbour->is_visited && !neighbour->is_obstacle)
                not_tested_nodes.push_back(neighbour);
            float possible_local_goal =
                node_current->local_goal + distance(node_current, neighbour);

            if (possible_local_goal < neighbour->local_goal) {
                neighbour->parent = node_current;
                neighbour->local_goal = possible_local_goal;

                neighbour->global_goal =
                    neighbour->local_goal + heuristic(neighbour, node_end);
            }
        }
    }
}
