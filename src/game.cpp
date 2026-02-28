#define OLC_PGE_APPLICATION
#include "olc/olcPixelGameEngine.h"

#define OLC_PGEX_TRANSFORMEDVIEW
#include "olc/olcPGEX_TransformedView.h"

#include "game.hpp"

#include "systems/rendering.hpp"
#include "systems/animation.hpp"

#include <ranges>
#include "engine/key_event.hpp"
#include "engine/mouse_event.hpp"

#include "utils/ecs.hpp"

#include "layers/game_layer.hpp"
#include "layers/menu_layer.hpp"
#include "layers/debug_layer.hpp"

TacticalGame::TacticalGame()
{
    sAppName = "TACTICAL LEFTIST";

    m_system_managers_amount = 0;
    m_decals_amount = 0;

    m_registry = std::make_unique<GameRegistry>();
}

bool TacticalGame::OnUserDestroy() { return true; }

bool TacticalGame::OnUserCreate()
{
    Warn("Loglevel is " << magic_enum::enum_name(Logger::get().loglevel()));
    
    Error("This is an error");
    Warn("This is a warning");
    Info("This is an info");
    Debug("This is a debug");
    Trace("This is a trace");

    Debug("Setting up camera");
    tvp = std::make_shared<olc::TileTransformedView>(
        olc::vi2d(ScreenWidth(), ScreenHeight()), olc::vi2d(1, 1));

    tvp->SetWorldScale({1.0f, 1.0f});
    tvp->SetWorldOffset(olc::vi2d(0.f, 0.f) -
                        (tvp->ScaleToWorld({ScreenWidth() / 2.f, ScreenHeight() / 2.f})));

    Component::World::Camera c;
    c.camera_pos = {0,0};
    c.world_scale = {0,0};
    c.is_panning = false;
    m_registry->set_world_component<Component::World::Camera>(c);
    camera_is_panning = false;
    Debug("Setting init state");

    add_layer<GameLayer>();
    // add_layer<MenuLayer>();
    // add_layer<DebugLayer>();

    Debug("Initiating systems");

    // TODO add helper function
    auto rendering_manager = std::make_unique<SystemManager>("Rendering System Manager");
    rendering_manager->add(std::make_unique<PreRenderer>());
    rendering_manager->add(std::make_unique<BackgroundRenderer>());
    rendering_manager->add(std::make_unique<MainRenderer>());
    rendering_manager->add(std::make_unique<WireframeRenderer>());
    rendering_manager->add(std::make_unique<GUIRenderer>());
    m_system_managers[m_system_managers_amount] = std::move(rendering_manager);
    Debug("Adding system " << m_system_managers[m_system_managers_amount++]->get_name());

    auto animation_manager = std::make_unique<SystemManager>("Animation System Manager");
    animation_manager->add(std::make_unique<BGAnimation>());
    animation_manager->add(std::make_unique<CharacterAnimation>());
    animation_manager->add(std::make_unique<GUIAnimation>());
    m_system_managers[m_system_managers_amount] = std::move(animation_manager);
    Debug("Adding system " << m_system_managers[m_system_managers_amount++]->get_name());


    prev_mouse_pos = GetMousePos();
    return true;
}

#include "components/animation.hpp"
bool TacticalGame::OnUserUpdate(float dt)
{
    // HACK for testing
    Clear(olc::BLACK);

    m_animation_tick = m_fElapsedTime > ANIMATION_TICK_TIME;
    if (m_animation_tick)
        m_fElapsedTime = 0;
    else
        m_fElapsedTime += dt;
    // HACK testing animation manager
    if (GetKey(olc::Key::P).bReleased) // && CURR_STATE->get_name()!="InitState") {
    {
        for (auto [ent, mng, list] : m_registry->get()
                                         .view<Component::Animation::AnimManager,
                                               Component::Animation::AnimationList>()
                                         .each()) {
            if (mng.index_curren_animation == 2)
                mng.index_curren_animation = 0;
            else
                mng.index_curren_animation++;
            mng.curr_animation = list.animations[mng.index_curren_animation];
            mng.index_curren_frame = 0;
        }
    }

    handle_inputs();

    // for (auto &layer : std::views::reverse(m_layers)) {
    for (auto &layer : m_layers) {
        layer->update();
        layer->draw();
    }

    for (int i = 0; i < m_system_managers_amount; ++i) {
        m_system_managers[i]->dispatch(this);
    }

    // dispatch possible events
    m_registry->update(dt);

    // HACK move to a camera system
    Component::World::Camera camera;
    auto has_camera = m_registry->get_world_component<Component::World::Camera>(camera);
    if(has_camera) {
        if (camera.is_panning) {
            auto pos_mouse = GetMousePos();
            get_tv()->UpdatePan(pos_mouse);
        }
        if (camera.is_panning && !camera_is_panning) {
            camera_is_panning = true;
            get_tv()->StartPan(GetMousePos());
        } else if (!camera.is_panning && camera_is_panning) {
            camera_is_panning = false;
            get_tv()->EndPan(GetMousePos());
        }
    }
    m_animation_tick = false;
    return true;
}

void TacticalGame::raise_event(Engine::Event &event)
{
    // traverse the states in descending order and give them the event
    // to let them handle it as they see fit
    for (auto &layer : std::views::reverse(m_layers)) {
        layer->on_event(event);
        if (event.consumed)
            break;
    }
}

void TacticalGame::handle_inputs()
{
    auto get_name = []() { return "game - handle_inputs()"; };

    // TODO basically what I can do is add a function callback
    // to be used instead of olc_UpdateMouseState so we don't have to
    // poll everything individually
    if (GetKey(olc::Key::L).bPressed) {
        Error("Pressed " << (int)olc::Key::L);
        Engine::KeyPressedEvent event((int)olc::Key::L, false);
        raise_event(event);
    }
    if (GetKey(olc::Key::L).bReleased) {
        Engine::KeyReleasedEvent event((int)olc::Key::L);
        raise_event(event);
    }
    if (GetMouse(MOUSE_LBUTTON).bPressed) {
        auto mouse_pos =
            get_tv()->ScaleToWorld(GetMousePos()) + get_tv()->GetWorldOffset();
        Engine::MouseButtonPressedEvent event(mouse_pos.x, mouse_pos.y,
                                              MOUSE_LBUTTON);
        raise_event(event);
    }
    if (GetMouse(MOUSE_LBUTTON).bReleased) {
        auto mouse_pos =
            get_tv()->ScaleToWorld(GetMousePos()) + get_tv()->GetWorldOffset();
        Engine::MouseButtonReleasedEvent event(mouse_pos.x, mouse_pos.y,
                                               MOUSE_LBUTTON);
        raise_event(event);
    }
    // if(GetMouse(MOUSE_LBUTTON).bHeld)     { Engine::MouseButtonPressedEvent
    // event(MOUSE_LBUTTON); raise_event(event); }
    if (GetMouse(MOUSE_RBUTTON).bPressed) {
        auto mouse_pos =
            get_tv()->ScaleToWorld(GetMousePos()) + get_tv()->GetWorldOffset();
        Engine::MouseButtonPressedEvent event(mouse_pos.x, mouse_pos.y,
                                              MOUSE_RBUTTON);
        raise_event(event);
    }
    if (GetMouse(MOUSE_RBUTTON).bReleased) {
        auto mouse_pos =
            get_tv()->ScaleToWorld(GetMousePos()) + get_tv()->GetWorldOffset();
        Engine::MouseButtonReleasedEvent event(mouse_pos.x, mouse_pos.y,
                                               MOUSE_RBUTTON);
        raise_event(event);
    }
    // if(GetMouse(MOUSE_RBUTTON).bHeld)     { Engine::MouseButtonPressedEvent
    // event(MOUSE_LBUTTON); raise_event(event); }
    if (GetMouse(MOUSE_MBUTTON).bPressed) {
        auto mouse_pos =
            get_tv()->ScaleToWorld(GetMousePos()) + get_tv()->GetWorldOffset();
        Engine::MouseButtonPressedEvent event(mouse_pos.x, mouse_pos.y,
                                              MOUSE_MBUTTON);
        raise_event(event);
    }
    if (GetMouse(MOUSE_MBUTTON).bReleased) {
        auto mouse_pos =
            get_tv()->ScaleToWorld(GetMousePos()) + get_tv()->GetWorldOffset();
        Engine::MouseButtonReleasedEvent event(mouse_pos.x, mouse_pos.y,
                                               MOUSE_MBUTTON);
        raise_event(event);
    }
    // if(GetMouse(MOUSE_MBUTTON).bHeld)     { Engine::MouseButtonPressedEvent
    // event(MOUSE_LBUTTON); raise_event(event); }
    //  if(GetMouseWheel() < 0)               { Engine::MouseButtonPressedEvent
    //  event(MOUSE_LBUTTON); raise_event(event); } if(GetMouseWheel() > 0) {
    //  Engine::MouseButtonPressedEvent event(MOUSE_LBUTTON); raise_event(event); }

    auto diff_mouse = prev_mouse_pos - GetMousePos();
    DrawLine(prev_mouse_pos, GetMousePos());
    if(min_distance < std::abs(diff_mouse.x) || min_distance < std::abs(diff_mouse.y)) {
        prev_mouse_pos = GetMousePos();
        auto mouse_pos = get_tv()->ScaleToWorld(prev_mouse_pos) + get_tv()->GetWorldOffset();
        Engine::MouseMovedEvent event(mouse_pos.x, mouse_pos.y);
        raise_event(event);
    }
}
