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

    auto sw = ge->ScreenWidth();
    auto sh = ge->ScreenHeight();

    for(float i = sw*0.1; i<sw*0.9; i+=sw*0.1) {
        for(float j = sh*0.1; j<sh*0.9; j+=sh*0.1) {
            ge->DrawRect(i,j, sw*0.1,sh*0.1);
        }
    }
}
