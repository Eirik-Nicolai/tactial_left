#include "initstate.hpp"

#include "starstate.hpp"
#include "loadstate.hpp"

#include "systems/rendering.hpp"

using namespace PlayingState;

InitState* InitState::m_state;

void InitState::init(TacticalGame* ge) {
    PRINT_FUNC

}
void InitState::cleanup(TacticalGame* ge) {
    PRINT_FUNC

}

void InitState::pause(TacticalGame* ge) {
    PRINT_FUNC

}
void InitState::resume(TacticalGame* ge) {
    PRINT_FUNC

}
void InitState::handle_input(TacticalGame* ge) {
    //PRINT_FUNC

    if(ge->GetMouse(MOUSE_LBUTTON).bReleased){
        ge->pop_state();
    }
}
void InitState::update(TacticalGame* ge) {
    //PRINT_FUNC
}

void InitState::enter(TacticalGame* ge) {
    PRINT_FUNC

}
void InitState::exit(TacticalGame* ge) {
    PRINT_FUNC

}

void InitState::draw(TacticalGame* ge) {
    //PRINT_FUNC

    render(ge->get_reg(), ge);
}
