#define OLC_PGE_APPLICATION
#include "olc/olcPixelGameEngine.h"

#define OLC_PGEX_TRANSFORMEDVIEW
#include "olc/olcPGEX_TransformedView.h"

#include "game.hpp"
#include "states/init/initstate.hpp"
#include "states/star/starstate.hpp"
#include "states/load/loadstate.hpp"

void TacticalGame::push_state(GameState* state) {
    TRACE_LOG_FUNC
    std::cout << "Pushing " << state->get_name() << std::endl;
    if(!m_states.empty()) {
        m_states.back()->pause(this);
    }

    m_states.push_back(state);
    m_states.back()->enter(this);
}
void TacticalGame::pop_state() {
    TRACE_LOG_FUNC
    if(!m_states.empty()) {
        TRACE_LOG_TEXT("List before" << m_states.size())
        m_states.back()->exit(this);
        m_states.pop_back();
    }

    TRACE_LOG_TEXT("List after" << m_states.size())
    if(!m_states.empty()) {
        m_states.back()->resume(this);
    }
}
void TacticalGame::change_state(GameState* state) {
    TRACE_LOG_FUNC
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

    // static auto offs = 100;
    // int i = 0;
    // for(auto state : m_states) {
    //     std::stringstream ss;
    //     ss << "i:" << i << " -> " << state->get_name();
    //     DrawString(100, 500 + (100*i), ss.str());
    //     ++i;
    // }

    return true;
}
