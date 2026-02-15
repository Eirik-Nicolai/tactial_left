#define OLC_PGE_APPLICATION
#include "olc/olcPixelGameEngine.h"

#define OLC_PGEX_TRANSFORMEDVIEW
#include "olc/olcPGEX_TransformedView.h"

#include "game.hpp"
// #include "states/init/initstate.hpp"
// #include "states/star/starstate.hpp"
// #include "states/load/loadstate.hpp"
#include "states/combat/combatstate.hpp"

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

    Debug("Setting init state");

    add_layer<GameLayer>();
    add_layer<MenuLayer>();
    add_layer<DebugLayer>();

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

    Debug("Loading sprite sheets");

    return true;
}

#include "components/animation.hpp"
void handle_inputs(TacticalGame *ge);
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
        for (auto [ent, mng, list] :
             m_reg.view<Animation::AnimManager, Animation::AnimationList>().each()) {
            if (mng.index_curren_animation == 2)
                mng.index_curren_animation = 0;
            else
                mng.index_curren_animation++;
            mng.curr_animation = list.animations[mng.index_curren_animation];
            mng.index_curren_frame = 0;
            Info("ANIMATION INDEX {} dur is {}", mng.index_curren_animation,
                 mng.curr_animation.frames[0].frame_duration);
        }
    }

    handle_inputs(this);

    // for (auto &layer : std::views::reverse(m_layers)) {
    for (auto &layer : m_layers) {
        layer->update(this);
        layer->draw(this);
    }

    for (int i = 0; i < m_system_managers_amount; ++i) {
        m_system_managers[i]->dispatch(this);
    }

    m_animation_tick = false;
    return true;
}

void TacticalGame::raise_event(Event &event)
{
    // traverse the states in descending order and give them the event
    // to let them handle it as they see fit
    for (auto &layer : std::views::reverse(m_layers)) {
        layer->on_event(this, event);
        if (event.consumed)
            break;
    }
}

void handle_inputs(TacticalGame *ge)
{
    // TODO basically what I can do is add a function callback
    // to be used instead of olc_UpdateMouseState so we don't have to
    // poll everything individually
    // if(ge->GetKey(olc::Key::P).bPressed)      { MouseButtonPressedEvent
    // event(MOUSE_LBUTTON); ge->raise_event(event); }
    // if(ge->GetKey(olc::Key::P).bReleased)     { MouseButtonPressedEvent
    // event(MOUSE_LBUTTON); ge->raise_event(event); }
    if (ge->GetMouse(MOUSE_LBUTTON).bPressed) {
        MouseButtonPressedEvent event(MOUSE_LBUTTON);
        ge->raise_event(event);
    }
    if (ge->GetMouse(MOUSE_LBUTTON).bReleased) {
        MouseButtonReleasedEvent event(MOUSE_LBUTTON);
        ge->raise_event(event);
    }
    // if(ge->GetMouse(MOUSE_LBUTTON).bHeld)     { MouseButtonPressedEvent
    // event(MOUSE_LBUTTON); ge->raise_event(event); }
    if (ge->GetMouse(MOUSE_RBUTTON).bPressed) {
        MouseButtonPressedEvent event(MOUSE_RBUTTON);
        ge->raise_event(event);
    }
    if (ge->GetMouse(MOUSE_RBUTTON).bReleased) {
        MouseButtonReleasedEvent event(MOUSE_RBUTTON);
        ge->raise_event(event);
    }
    // if(ge->GetMouse(MOUSE_RBUTTON).bHeld)     { MouseButtonPressedEvent
    // event(MOUSE_LBUTTON); ge->raise_event(event); }
    if (ge->GetMouse(MOUSE_MBUTTON).bPressed) {
        MouseButtonPressedEvent event(MOUSE_MBUTTON);
        ge->raise_event(event);
    }
    if (ge->GetMouse(MOUSE_MBUTTON).bReleased) {
        MouseButtonReleasedEvent event(MOUSE_MBUTTON);
        ge->raise_event(event);
    }
    // if(ge->GetMouse(MOUSE_MBUTTON).bHeld)     { MouseButtonPressedEvent
    // event(MOUSE_LBUTTON); ge->raise_event(event); }
    //  if(ge->GetMouseWheel() < 0)               { MouseButtonPressedEvent
    //  event(MOUSE_LBUTTON); ge->raise_event(event); } if(ge->GetMouseWheel() > 0) {
    //  MouseButtonPressedEvent event(MOUSE_LBUTTON); ge->raise_event(event); }
}
