#include "combatstate_initialize.hpp"
#include "combatstate_playermovement.hpp"
#include "utils/ecs.hpp"
#include "utils/geometry.hpp"
#include "utils/complex_datatypes.hpp"

#include "asset_manager.hpp"

using namespace TransitionState;

CombatStateInit::CombatStateInit(TacticalGame *ge, std::shared_ptr<GameRegistry> reg)
    : PlayingState::CombatState(ge, reg)
{
    LOG_FUNC
    handler = std::make_unique<InputHandler>();

    screen_w = ge->ScreenWidth();
    screen_h = ge->ScreenHeight();
    auto w = (0.04);
    auto h = (0.05);
    rect_w = screen_w * w;
    rect_h = screen_h * h;

    reg->set_world_component<Component::Combat::World::TileInfo>(rect_w, rect_h);

    Debug("Entering combat state");
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

            Interaction::Selectable select;
            select.on_left_select = nullptr;
            select.on_middle_select = nullptr;
            select.on_right_select = [this,
                                      name = ss.str()](std::shared_ptr<GameRegistry> reg,
                                                       entt::entity me) {
                auto get_name = [name]() { return name; };
                auto node = reg->unsafe_get_component<Combat::Node>(me);
                node.is_obstacle = !node.is_obstacle;

                return true;
            };
            reg->add_component<Interaction::Selectable>(tile, select);
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
    std::string player_decal;
    try {
        player_decal = AssetManager::instance().load_sprite_sheet(path_1, "User Sprite sheet 1");
    } catch (std::exception &e) {
        Error("Unable to load image at path " << path_1);
        throw e;
    }
    
    Debug("LOADED SPRITE " << player_decal);

    reg->add_component<Pos>(player, 50.f, 50.f);
    reg->add_component<Size>(player, 300.f, 300.f);
   
    Debug("creating rendering manager");
    reg->add_component<Rendering::Spritesheet>(player, player_decal, Size(32,32));

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
    reg->unsafe_set_component<Component::Pos>(player, [&](Component::Pos &p) {
        p = (*reg->get_component<Component::Pos>(combat_tiles[0]));
    });

    Info("Finished initialization");
}

void CombatStateInit::update(){
}

void CombatStateInit::handle_input(Engine::Event& event) {
        Engine::EventDispatcher dispatcher(event);
    dispatcher.Dispatch<Engine::KeyReleasedEvent>(
        [this](Engine::KeyReleasedEvent &e) {
            return key_released(e);
        });
}

bool CombatStateInit::key_released(Engine::KeyReleasedEvent &event)
{
    if (event.get_key() == olc::Key::SPACE) {
        m_change_state_to<PlayingState::CombatStatePlayerMovement>();
    }
    return false;
}
