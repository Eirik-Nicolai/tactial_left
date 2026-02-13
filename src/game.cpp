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

// TODO keep 2 lists of states for one
// current_states one next states and
// change at the end of the game update func
// void TacticalGame::push_state(GameState* state) {
//     LOG_FUNC
//     Debug("Pushing state {}", state->get_name());
//     if(!m_states.empty()) {
//         Debug("Pausing state {}", m_states.back()->get_name());
//         m_states.back()->pause(this);
//     }

//     m_states.push_back(state);
//     m_states.back()->enter(this);
// }
// void TacticalGame::pop_state() {
//     LOG_FUNC
//     if(!m_states.empty()) {
//         Debug("List before {}", m_states.size());
//         m_states.back()->exit(this);
//         m_states.pop_back();
//     }

//     Debug("List after {}", m_states.size());
//     if(!m_states.empty()) {
//         m_states.back()->resume(this);
//     }
// }
// void TacticalGame::change_state(GameState* state) {
//     LOG_FUNC
//     // cleanup all the current states
//     Debug("Cleaning up {} states", m_states.size());
//     while ( !m_states.empty() ) {
//         m_states.back()->exit(this);
//         Debug("Exiting state {}", m_states.back()->get_name());
//         m_states.pop_back();
//     }

//     Debug("Changing to state {}", state->get_name());
//     // store and init the new state
//     m_states.push_back(state);
//     m_states.back()->enter(this);
// }

TacticalGame::TacticalGame()
{
    sAppName = "TACTICAL LEFTIST";

    m_system_managers_amount = 0;
}

bool TacticalGame::OnUserDestroy() {
    // PlayingState::InitState::Instance()->cleanup(this);
    // PlayingState::CombatState::Instance()->cleanup(this);
    // PlayingState::StarState::Instance()->cleanup(this);
    // TransitionState::LoadState::Instance()->cleanup(this);

    return true;
}

bool TacticalGame::OnUserCreate()
{
    // TODO maybe change these to be
    // stored in a list inside the GE ?
    Debug("Initializing game states");
    // PlayingState::InitState::Instance()->init(this);
    // PlayingState::StarState::Instance()->init(this);
    // PlayingState::CombatState::Instance()->init(this);
    // TransitionState::LoadState::Instance()->init(this);

    Error("Printing {}", fmt::ptr(PlayingState::StarStateSelected::Instance()));

    Debug("Setting up camera");
    tvp = std::make_shared<olc::TileTransformedView>(
        olc::vi2d( ScreenWidth(), ScreenHeight()),
        olc::vi2d(1, 1)
    );

    tvp->SetWorldScale({1.0f, 1.0f});
    tvp->SetWorldOffset(olc::vi2d(0.f, 0.f) - (tvp->ScaleToWorld({ScreenWidth()/2.f,ScreenHeight()/2.f})));

    Debug("Setting init state");

    add_layer<GameLayer>();
    add_layer<MenuLayer>();
    add_layer<DebugLayer>();
    //change_state(PlayingState::CombatState::Instance());
    //change_state(TransitionState::LoadState::Instance());
    
    // if(!m_states.empty()) {
    //     Info("Starting on state {}", m_states.front()->get_name());
    // }

    Debug("Initiating systems");

    // TODO add helper function
    auto rendering_manager = std::make_unique<SystemManager>("Rendering System Manager");
    rendering_manager->add(std::make_unique<PreRenderer>());
    rendering_manager->add(std::make_unique<BackgroundRenderer>());
    rendering_manager->add(std::make_unique<MainRenderer>());
    rendering_manager->add(std::make_unique<WireframeRenderer>());
    rendering_manager->add(std::make_unique<GUIRenderer>());
    // m_system_managers[m_system_managers_amount] = std::move(rendering_manager);
    // Debug("Adding system {}", m_system_managers[m_system_managers_amount++]->get_name());

    auto animation_manager = std::make_unique<SystemManager>("Animation System Manager");
    animation_manager->add(std::make_unique<BGAnimation>());
    animation_manager->add(std::make_unique<CharacterAnimation>());
    animation_manager->add(std::make_unique<GUIAnimation>());
    // m_system_managers[m_system_managers_amount] = std::move(animation_manager);
    // Debug("Adding system {}", m_system_managers[m_system_managers_amount++]->get_name());

    Debug("Loading sprite sheets");

    return true;
}

void handle_inputs(TacticalGame* ge);
bool TacticalGame::OnUserUpdate(float dt)
{
    // HACK for testing
    Clear(olc::BLACK);

    m_animation_tick = m_fElapsedTime > ANIMATION_TICK_TIME;
    if(m_animation_tick) m_fElapsedTime = 0;
    else m_fElapsedTime += dt;

    // HACK testing animation manager
    if(GetKey(olc::Key::P).bReleased) // && CURR_STATE->get_name()!="InitState") {
    {
        // //push_state(PlayingState::InitState::Instance());
        // for(auto [ent, mng] : m_reg.view<Animation::AnimManager>().each()) {
        //     Rendering::Spritesheet sheet;
        //     tryget_component(m_reg, mng.sprite_sheet, sheet);
        //     if(mng.index_curren_animation==2) mng.index_curren_animation=0;
        //     else mng.index_curren_animation++;
        //     mng.curr_animation = sheet.animations[mng.index_curren_animation];
        //     mng.index_curren_frame = 0;
        //     Info("ANIMATION INDEX {} dur is {}", mng.index_curren_animation, mng.curr_animation.frames[0].frame_duration);
        // }
    }

    handle_inputs(this);
    
    //for (auto &layer : std::views::reverse(m_layers)) {
    for (auto &layer : m_layers) {
        layer->update(this);
        layer->draw(this);
    }
    
    for(int i = 0; i < m_system_managers_amount; ++i) {
        m_system_managers[i]->dispatch(this);
    }

    m_animation_tick = false;
    return true;
}


void TacticalGame::raise_event(Event& event) {
    // traverse the states in descending order and give them the event
    // to let them handle it as they see fit
    for (auto &layer : std::views::reverse(m_layers)) {
      layer->on_event(this, event);
      if (event.consumed)
        break;
    }
}

void handle_inputs(TacticalGame* ge) {
    // TODO basically what I can do is add a function callback
    // to be used instead of olc_UpdateMouseState so we don't have to
    // poll everything individually
    // if(ge->GetKey(olc::Key::P).bPressed)      { MouseButtonPressedEvent event(MOUSE_LBUTTON); ge->raise_event(event); }
    // if(ge->GetKey(olc::Key::P).bReleased)     { MouseButtonPressedEvent event(MOUSE_LBUTTON); ge->raise_event(event); }
    if(ge->GetMouse(MOUSE_LBUTTON).bPressed)  { MouseButtonPressedEvent event(MOUSE_LBUTTON); ge->raise_event(event); }
    if(ge->GetMouse(MOUSE_LBUTTON).bReleased) { MouseButtonReleasedEvent event(MOUSE_LBUTTON); ge->raise_event(event); }
    //if(ge->GetMouse(MOUSE_LBUTTON).bHeld)     { MouseButtonPressedEvent event(MOUSE_LBUTTON); ge->raise_event(event); }
    if(ge->GetMouse(MOUSE_RBUTTON).bPressed)  { MouseButtonPressedEvent event(MOUSE_RBUTTON); ge->raise_event(event); }
    if(ge->GetMouse(MOUSE_RBUTTON).bReleased) { MouseButtonReleasedEvent event(MOUSE_RBUTTON); ge->raise_event(event); }
    //if(ge->GetMouse(MOUSE_RBUTTON).bHeld)     { MouseButtonPressedEvent event(MOUSE_LBUTTON); ge->raise_event(event); }
    if(ge->GetMouse(MOUSE_MBUTTON).bPressed)  { MouseButtonPressedEvent event(MOUSE_MBUTTON); ge->raise_event(event); }
    if(ge->GetMouse(MOUSE_MBUTTON).bReleased) { MouseButtonReleasedEvent event(MOUSE_MBUTTON); ge->raise_event(event); }
    //if(ge->GetMouse(MOUSE_MBUTTON).bHeld)     { MouseButtonPressedEvent event(MOUSE_LBUTTON); ge->raise_event(event); }
    // if(ge->GetMouseWheel() < 0)               { MouseButtonPressedEvent event(MOUSE_LBUTTON); ge->raise_event(event); }
    // if(ge->GetMouseWheel() > 0)               { MouseButtonPressedEvent event(MOUSE_LBUTTON); ge->raise_event(event); }

}
