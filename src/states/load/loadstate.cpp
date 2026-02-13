#include "loadstate.hpp"

#include "states/init/initstate.hpp"
#include "states/star/starstate.hpp"
#include "states/combat/combatstate.hpp"

#include "components/components.hpp"
#include "logger.hpp"

#include <thread>
#include <cstdlib>

#define LOADING_ELEMENTS 2

using namespace TransitionState;
LoadState* LoadState::m_state;

unsigned bounded_rand(unsigned upper_range = 2)
{
    for (unsigned x, r;;)
        if (x = rand(), r = x % upper_range, x - r <= -upper_range)
            return r;
}

void sleep_for(int seconds) {
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
}

void do_work(std::vector<int>& assets) {
    sleep_for(bounded_rand());
    for(auto i = 0; i < LOADING_ELEMENTS; ++i) {
        //Trace("Adding element");
        assets.emplace_back(i);
        sleep_for(bounded_rand());
    }
    //Debug("Finished thread");
}

void LoadState::init(TacticalGame* ge) {
    LOG_FUNC
    olc::Sprite* s;

    olc::Decal d = olc::Decal(s);

    ge->SetDrawTarget(1);
}

void LoadState::cleanup(TacticalGame* ge) {
    LOG_FUNC
}

void LoadState::enter(TacticalGame* ge) {
    LOG_FUNC
    sElapsedTime = 0;
    m_assets.clear();

    // Debug("Making thread");
    // m_tLoader = std::thread(do_work, std::ref(m_assets));
    // m_tLoader.detach();

    // auto& reg = ge->get_reg();
    // auto sun = reg.create();
    // reg.emplace<Pos>(sun, 0.f, 0.f);
    // reg.emplace<Rendering::Wireframe>(sun, Rendering::Wireframe::TYPE::CIRCLE_FILL, olc::DARK_RED);
    // reg.emplace<Size>(sun, 60.f, 0.f);

    // auto earth = reg.create();
    // reg.emplace<Pos>(earth, 50.f, 50.f );
    // reg.emplace<Rendering::Wireframe>(earth, Rendering::Wireframe::TYPE::TRIANGLE_FILL, olc::DARK_BLUE);
    // reg.emplace<Size>(earth, 30.f, 0.f);
    // // reg.emplace<_selectable>(earth);
    // // reg.emplace<_hoverable>(earth);

    // auto moon = reg.create();
    // reg.emplace<Pos>(moon, 100.f, 100.f );
    // reg.emplace<Rendering::Wireframe>(moon,Rendering::Wireframe::TYPE::SQUARE, olc::WHITE);
    // reg.emplace<Size>(moon, 10.f, 10.f);
    // // reg.emplace<_selectable>(moon);
    // // reg.emplace<_hoverable>(moon);

    // reg.emplace<Debugging::Debug>(sun, "SUN");
    // reg.emplace<Debugging::Debug>(earth, "SPRITE SHEET");
    // reg.emplace<Debugging::Debug>(moon, "PLAYER");

    // auto path_1 = "assets/Cute_Fantasy_Free/Player/Player.png";
    // auto spritesheet_player = reg.create();
    // auto player_decal_index = ge->load_decal(path_1, false, true);
    // Debug("LOADED SPRITE {}", player_decal_index);

    // reg.emplace<Debugging::Debug>(spritesheet_player, "SPRITE SHEET");

    // Debug("creating anim idle");
    // Rendering::Animation::SpriteSheetAnimation idle_animation;
    // idle_animation.name = "idle";
    // idle_animation.frames[0] = Rendering::Animation::AnimationFrame{Pos{0,0}, 1};
    // idle_animation.frames[1] = Rendering::Animation::AnimationFrame{Pos{1,0}, 1};
    // idle_animation.frames[2] = Rendering::Animation::AnimationFrame{Pos{2,0}, 1};
    // idle_animation.frames[3] = Rendering::Animation::AnimationFrame{Pos{3,0}, 1};
    // idle_animation.frames[4] = Rendering::Animation::AnimationFrame{Pos{4,0}, 1};
    // idle_animation.frames[5] = Rendering::Animation::AnimationFrame{Pos{5,0}, 1};
    // idle_animation.is_looping = true;
    // idle_animation.frame_animation_length = 6;

    // Debug("creating anim walking");
    // Rendering::Animation::SpriteSheetAnimation walking_east;
    // walking_east.name = "walking_east";
    // walking_east.frames[0] = Rendering::Animation::AnimationFrame{Pos{0,4}, 1};
    // walking_east.frames[1] = Rendering::Animation::AnimationFrame{Pos{1,4}, 1};
    // walking_east.frames[2] = Rendering::Animation::AnimationFrame{Pos{2,4}, 1};
    // walking_east.frames[3] = Rendering::Animation::AnimationFrame{Pos{3,4}, 1};
    // walking_east.frames[4] = Rendering::Animation::AnimationFrame{Pos{4,4}, 1};
    // walking_east.frames[5] = Rendering::Animation::AnimationFrame{Pos{5,4}, 1};
    // walking_east.is_looping = true;
    // walking_east.frame_animation_length = 6;

    // Debug("creating anim dead");
    // Rendering::Animation::SpriteSheetAnimation dead;
    // dead.name = "dead";
    // dead.frames[0] = Rendering::Animation::AnimationFrame{Pos{0,9}, 1};
    // dead.frames[1] = Rendering::Animation::AnimationFrame{Pos{1,9}, 1};
    // dead.frames[2] = Rendering::Animation::AnimationFrame{Pos{2,9}, 1};
    // dead.frames[3] = Rendering::Animation::AnimationFrame{Pos{3,9}, 1};
    // dead.is_looping = false;
    // dead.frame_animation_length = 4;

    // Debug("Created sprite sheet with animation {}", idle_animation.name);
    // Rendering::Spritesheet sprite_sheet;
    // sprite_sheet.decal_index = player_decal_index;
    // sprite_sheet.pixel_frame_size = Size{32,32};
    // sprite_sheet.animations[sprite_sheet.animations_amt++] = idle_animation;
    // sprite_sheet.animations[sprite_sheet.animations_amt++] = walking_east;
    // sprite_sheet.animations[sprite_sheet.animations_amt++] = dead;
    // reg.emplace<Rendering::Spritesheet>(spritesheet_player, sprite_sheet);

    // Debug("creating anim manager");
    // Rendering::Animation::AnimManager mng;
    // mng.curr_animation = idle_animation;
    // mng.index_curren_animation = 0;
    // mng.index_curren_frame = 0;
    // mng.name = "ANIM MANAGER";
    // mng.frames_elapsed = 0;
    // mng.sprite_sheet = spritesheet_player;
    // reg.emplace<Rendering::Animation::AnimManager>(moon, mng);

    // Debug("creating rendering manager");
    // Rendering::RenderingManager rendering_manager;
    // rendering_manager.sprite_sheet = spritesheet_player;
    // rendering_manager.pos_sprite_sheet = {0.f, 0.f};
    // rendering_manager.sprite_scale = {1.f, 1.f};
    // rendering_manager.index_decal = player_decal_index;
    // reg.emplace<Rendering::RenderingManager>(moon, rendering_manager);

    // Debug("Rendering manager info {} {} {}", rendering_manager.index_decal,
    //       rendering_manager.pos_sprite_sheet,
    //       rendering_manager.sprite_scale);

    // reg.emplace<Rendering::Layer::_first>(moon);

    // reg.emplace<Orbiting>(earth, sun, 500.f, 0.1f);
    // reg.emplace<Orbiting>(moon, earth, 50.f, -0.5f);

    // auto path_2 = "assets/Cute_Fantasy_Free/Enemies/Skeleton.png";
    // auto skeleton_decal_index = ge->load_decal(path_2, false, true);
    // reg.emplace<Rendering::Decal>(earth, skeleton_decal_index, Screen::Size{400,400}, Screen::Pos{40,40});
    // reg.emplace<Rendering::Layer::_second>(earth);
    // Debug("LOADED SPRITE {}", skeleton_decal_index);

    // auto path_3 = "assets/Cute_Fantasy_Free/Enemies/Slime_Green.png";
    // auto slime_decal_index = ge->load_decal(path_3, false, true);
    // reg.emplace<Rendering::Decal>(sun, slime_decal_index, Screen::Size{400,400}, Screen::Pos{40,40});
    // reg.emplace<Rendering::Layer::_third>(sun);
    // Debug("LOADED SPRITE {}", slime_decal_index);

}
void LoadState::exit(TacticalGame* ge) {
    LOG_FUNC
}

void LoadState::pause(TacticalGame* ge) {
    LOG_FUNC

}
void LoadState::resume(TacticalGame* ge) {
    LOG_FUNC

}
void LoadState::handle_input(TacticalGame* ge) {
    //LOG_FUNC

    if(ge->GetMouse(MOUSE_LBUTTON).bReleased) {
        //TRACE_LOG_TEXT("Loading, please wait")
    }
}
void LoadState::update(TacticalGame* ge) {
    //TRACE_LOG_TEXT_NOL(sElapsedTime)

    sElapsedTime += ge->GetElapsedTime();
    // if(m_assets.size() >= LOADING_ELEMENTS) {
    // }

    if(sElapsedTime > 1) { // 3 second wait to simulate loading
        // ge->change_state(PlayingState::CombatState::Instance());
    }
}

void LoadState::draw(TacticalGame* ge) {
    //LOG_FUNC

    if(m_assets.size() >= LOADING_ELEMENTS) {
        ge->DrawString(400, 300, "DONE",olc::RED, 3);
    } else {
        ge->DrawString(400, 300, "LOADING, PLEASE WAIT...",olc::RED, 3);
    }

    int width = 1200;
    float factor = (m_assets.size()/5.f);

    ge->FillRect(200, 500, (width*factor), 50, olc::RED);
    ge->DrawRect(200, 500, width, 50);

    ge->DrawString({20,30}, std::to_string(factor));
    ge->DrawString({20,50}, std::to_string(m_assets.size()));
}
