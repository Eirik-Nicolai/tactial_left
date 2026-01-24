#include "initstate.hpp"

#include "states/load/loadstate.hpp"
#include "states/star/starstate.hpp"

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
    auto tv = ge->get_tv();

    auto w = ge->ScreenWidth()*0.8;
    auto h = ge->ScreenHeight()*0.8;

    ge->FillRect(ge->ScreenWidth()*0.1, ge->ScreenHeight()*0.1, w, h, olc::BLACK);
    ge->DrawRect(ge->ScreenWidth()*0.1, ge->ScreenHeight()*0.1, w, h, olc::WHITE);
    ge->DrawString({ge->ScreenWidth()/2,ge->ScreenHeight()/2}, "P", olc::RED, 4);

}
