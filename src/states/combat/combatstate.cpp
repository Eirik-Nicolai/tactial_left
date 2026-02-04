#include "combatstate.hpp"

#include "states/load/loadstate.hpp"
#include "states/star/starstate.hpp"

#include "systems/rendering.hpp"
#include "logger.hpp"

using namespace PlayingState;
CombatState* CombatState::m_state;

void CombatState::init(TacticalGame* ge) {
    LOG_FUNC

}
void CombatState::cleanup(TacticalGame* ge) {
    LOG_FUNC

}

void CombatState::pause(TacticalGame* ge) {
    LOG_FUNC

}
void CombatState::resume(TacticalGame* ge) {
    LOG_FUNC

}

void CombatState::enter(TacticalGame* ge) {
    LOG_FUNC

}
void CombatState::exit(TacticalGame* ge) {
    LOG_FUNC

}

void CombatState::handle_input(TacticalGame* ge) {
    //LOG_FUNC
}
void CombatState::update(TacticalGame* ge) {
    //LOG_FUNC
}

void CombatState::draw(TacticalGame* ge) {
    //LOG_FUNC
    auto tv = ge->get_tv();
    auto tile_amt_x = 20;
    auto tile_amt_y = 18;
    auto sw = ge->ScreenWidth();
    auto sh = ge->ScreenHeight();
    auto w = (0.04);
    auto h = (0.05);
    auto rect_w = sw*w;
    auto rect_h = sh*h;
    auto offs_x = (sw/2) - (rect_w*tile_amt_x/2);
    auto offs_y = (sh/2) - (rect_h*tile_amt_y/2);

    for(auto x = 0; x < tile_amt_x; x += 1) {
        for(auto y = 0; y < tile_amt_y; y += 1) {
            ge->DrawRect(offs_x + (x*rect_w), offs_y + (y*rect_h), rect_w, rect_h);
        }
    }
}
