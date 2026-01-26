#define OLC_PGE_APPLICATION
#include "olc/olcPixelGameEngine.h"

#define OLC_PGEX_TRANSFORMEDVIEW
#include "olc/olcPGEX_TransformedView.h"

#include "game.hpp"
#include "states/init/initstate.hpp"
#include "states/star/starstate.hpp"
#include "states/load/loadstate.hpp"

void TacticalGame::push_state(GameState* state) {
    LOG_FUNC
    std::cout << "Pushing " << state->get_name() << std::endl;
    if(!m_states.empty()) {
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
    while ( !m_states.empty() ) {
        m_states.back()->exit(this);
        m_states.pop_back();
    }

    // store and init the new state
    m_states.push_back(state);
    m_states.back()->enter(this);
}

TacticalGame::TacticalGame()
{
    sAppName = "TACTICAL LEFTIST";

    // spdlog::info("Welcome to spdlog!");
    // spdlog::error("Some error message with arg: {}", 1);
    Logger::Get()->set_log_level(spdlog::level::trace);

    Error("This is a error line number {}", 12);
    Warn("This is a warning line number {}", 12);
    Info("This is a info line number {}", 12);
    Debug("This is a debug line number {}", 12);
    Trace("This is a trace line number {}", 12);
}

bool TacticalGame::OnUserDestroy() {
    std::cout << "\e[?25h";

    PlayingState::InitState::Instance()->cleanup(this);
    PlayingState::StarState::Instance()->cleanup(this);
    TransitionState::LoadState::Instance()->cleanup(this);

    return true;
}

bool TacticalGame::OnUserCreate()
{
    // hide terminal cursor
    std::cout << "\e[?25l";

    // TODO maybe change these to be
    // stored in a list inside the GE ?
    PlayingState::InitState::Instance()->init(this);
    PlayingState::StarState::Instance()->init(this);
    TransitionState::LoadState::Instance()->init(this);

    tvp = std::make_shared<olc::TileTransformedView>(
        olc::vi2d( ScreenWidth(), ScreenHeight()),
        olc::vi2d(1, 1));
    tvp->SetWorldScale({1.0f, 1.0f});
    tvp->SetWorldOffset(olc::vi2d(0.f, 0.f) - (tvp->ScaleToWorld({ScreenWidth()/2.f,ScreenHeight()/2.f})));

    change_state(TransitionState::LoadState::Instance());
    if(!m_states.empty()) {
        std::cout << "LOADED STATE" << std::endl;
    }

    return true;
}

bool TacticalGame::OnUserUpdate(float dt)
{
    Clear(olc::BLACK);

    auto CURR_STATE = m_states.back();
    if(CURR_STATE) {
        CURR_STATE->handle_input(this);
        CURR_STATE->update(this);
    }
    for(auto &state : m_states) {
        state->draw(this);
    }

    return true;
}
