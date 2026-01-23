#include "starstate.hpp"

StarState* StarState::m_state;

void StarState::init(TacticalGame* ge) {
    PRINT_TEXT(5);

    number = 47;
}
void StarState::cleanup(TacticalGame* ge) {
    PRINT_FUNC

}

void StarState::enter(TacticalGame* ge) {
    PRINT_FUNC

}
void StarState::exit(TacticalGame* ge) {
    PRINT_FUNC
}

void StarState::pause(TacticalGame* ge) {
    PRINT_FUNC

}
void StarState::resume(TacticalGame* ge) {
    PRINT_FUNC

}
void StarState::handle_input(TacticalGame* ge) {
    //PRINT_FUNC

    if(ge->GetMouse(0).bReleased){
        ge->pop_state();
    }
}
void StarState::update(TacticalGame* ge) {
    //PRINT_FUNC
    // std::cout << get_name() << " -> " << __func__ << "\t\t\t\r";
}
void StarState::draw(TacticalGame* ge) {
    //PRINT_FUNC
    std::stringstream ss;
    ss << number;
    ge->DrawString(700, 100, ss.str());
    ge->DrawString(700, 300, "THIS IS A TEXT");
    ge->DrawString(700, 600, "THIS IS A TEXT");
}
