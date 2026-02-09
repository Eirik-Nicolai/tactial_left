#define OLC_PGE_APPLICATION
#include "olc/olcPixelGameEngine.h"

#define OLC_PGEX_TRANSFORMEDVIEW
#include "olc/olcPGEX_TransformedView.h"

#include "game.hpp"
#include "states/init/initstate.hpp"
#include "states/star/starstate.hpp"
#include "states/load/loadstate.hpp"
#include "states/combat/combatstate.hpp"

#include "systems/rendering.hpp"
#include "systems/animation.hpp"


#include "utils/ecs.hpp"

// TODO keep 2 lists of states for one
// current_states one next states and
// change at the end of the game update func
void TacticalGame::push_state(GameState* state) {
    LOG_FUNC
    Debug("Pushing state {}", state->get_name());
    if(!m_states.empty()) {
        Debug("Pausing state {}", m_states.back()->get_name());
        m_states.back()->pause(this);
    }

    m_states.push_back(state);
    m_states.back()->enter(this);
}
void TacticalGame::pop_state() {
    LOG_FUNC
    if(!m_states.empty()) {
        Debug("List before {}", m_states.size());
        m_states.back()->exit(this);
        m_states.pop_back();
    }

    Debug("List after {}", m_states.size());
    if(!m_states.empty()) {
        m_states.back()->resume(this);
    }
}
void TacticalGame::change_state(GameState* state) {
    LOG_FUNC
    // cleanup all the current states
    Debug("Cleaning up {} states", m_states.size());
    while ( !m_states.empty() ) {
        m_states.back()->exit(this);
        Debug("Exiting state {}", m_states.back()->get_name());
        m_states.pop_back();
    }

    Debug("Changing to state {}", state->get_name());
    // store and init the new state
    m_states.push_back(state);
    m_states.back()->enter(this);
}

TacticalGame::TacticalGame()
{
    sAppName = "TACTICAL LEFTIST";

    m_system_managers_amount = 0;
}

bool TacticalGame::OnUserDestroy() {
    PlayingState::InitState::Instance()->cleanup(this);
    PlayingState::CombatState::Instance()->cleanup(this);
    PlayingState::StarState::Instance()->cleanup(this);
    TransitionState::LoadState::Instance()->cleanup(this);

    return true;
}

bool TacticalGame::OnUserCreate()
{
    // TODO maybe change these to be
    // stored in a list inside the GE ?
    Debug("Initializing game states");
    PlayingState::InitState::Instance()->init(this);
    PlayingState::StarState::Instance()->init(this);
    PlayingState::CombatState::Instance()->init(this);
    TransitionState::LoadState::Instance()->init(this);

    Error("Printing {}", fmt::ptr(PlayingState::StarStateSelected::Instance()));

    Debug("Setting up camera");
    tvp = std::make_shared<olc::TileTransformedView>(
        olc::vi2d( ScreenWidth(), ScreenHeight()),
        olc::vi2d(1, 1)
    );

    tvp->SetWorldScale({1.0f, 1.0f});
    tvp->SetWorldOffset(olc::vi2d(0.f, 0.f) - (tvp->ScaleToWorld({ScreenWidth()/2.f,ScreenHeight()/2.f})));

    Debug("Setting init state");
    //change_state(PlayingState::CombatState::Instance());
    change_state(TransitionState::LoadState::Instance());
    if(!m_states.empty()) {
        Info("Starting on state {}", m_states.front()->get_name());
    }

    Debug("Initiating systems");

    // TODO add helper function
    auto rendering_manager = std::make_unique<SystemManager>("Rendering System Manager");
    rendering_manager->add(std::make_unique<PreRenderer>());
    rendering_manager->add(std::make_unique<BackgroundRenderer>());
    rendering_manager->add(std::make_unique<MainRenderer>());
    rendering_manager->add(std::make_unique<WireframeRenderer>());
    rendering_manager->add(std::make_unique<GUIRenderer>());
    m_system_managers[m_system_managers_amount] = std::move(rendering_manager);
    Debug("Adding system {}", m_system_managers[m_system_managers_amount++]->get_name());

    auto animation_manager = std::make_unique<SystemManager>("Animation System Manager");
    animation_manager->add(std::make_unique<BGAnimation>());
    animation_manager->add(std::make_unique<CharacterAnimation>());
    animation_manager->add(std::make_unique<GUIAnimation>());
    m_system_managers[m_system_managers_amount] = std::move(animation_manager);
    Debug("Adding system {}", m_system_managers[m_system_managers_amount++]->get_name());

    Debug("Loading sprite sheets");

    return true;
}

#include "components/rendering.hpp"
#include "components/animation.hpp"
bool TacticalGame::OnUserUpdate(float dt)
{
    // HACK for testing
    Clear(olc::BLACK);

    m_animation_tick = m_fElapsedTime > ANIMATION_TICK_TIME;
    if(m_animation_tick) m_fElapsedTime = 0;
    else m_fElapsedTime += dt;

    auto CURR_STATE = m_states.back();

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

    if(CURR_STATE) {
        CURR_STATE->handle_input(this);
        CURR_STATE->update(this);
    }
    for(auto &state : m_states) {
        state->draw(this);
    }

    for(int i = 0; i < m_system_managers_amount; ++i) {
        m_system_managers[i]->dispatch(this);
    }

    m_animation_tick = false;
    return true;
}
