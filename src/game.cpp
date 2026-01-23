#define OLC_PGE_APPLICATION
#include "game.hpp"
#include "states/initstate.hpp"
#include "states/starstate.hpp"

void TacticalGame::push_state(GameState* state) {
    PRINT_FUNC
    std::cout << "Pushing " << state->get_name() << std::endl;
    if(!m_states.empty()) {
        m_states.back()->pause(this);
    }

    m_states.push_back(state);
    m_states.back()->enter(this);

}
void TacticalGame::pop_state() {
    PRINT_FUNC
    if(!m_states.empty()) {
        m_states.back()->exit(this);
        m_states.pop_back();
    }

    if(!m_states.empty()) {
        m_states.back()->resume(this);
    }
}
void TacticalGame::change_state(GameState* state) {
    PRINT_FUNC
    // cleanup the current state
    if ( !m_states.empty() ) {
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

bool TacticalGame::OnUserCreate()
{
    // hide terminal cursor
    std::cout << "\e[?25l";

    InitState::get()->init(this);
    StarState::get()->init(this);

    change_state(InitState::get());
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
        CURR_STATE->draw(this);
    }

    static auto offs = 100;
    int i = 0;
    for(auto state : m_states) {
        std::stringstream ss;
        ss << "i:" << i << " -> " << state->get_name();
        DrawString(100, 500 + (100*i), ss.str());
        ++i;
    }

    return true;
}
