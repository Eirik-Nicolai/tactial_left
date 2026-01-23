#include "initstate.hpp"

#include "starstate.hpp"

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

    if(ge->GetMouse(0).bReleased) {
        ge->push_state(StarState::get());
    }
}
void InitState::update(TacticalGame* ge) {
    //PRINT_FUNC

    auto i = ge->GetElapsedTime();

}

void InitState::enter(TacticalGame* ge) {
    PRINT_FUNC

}
void InitState::exit(TacticalGame* ge) {
    PRINT_FUNC

}

void InitState::draw(TacticalGame* ge) {
    //PRINT_FUNC
    PRINT_TEXT_NOL("Hello")
    static int offs = 50;
    static int w = 80;
    static int h = 50;

    for(auto i = 0; i < 16; ++i) {
        for(auto j = 0; j < 16; ++j) {
            ge->DrawRect(offs+(i*w),
                         offs+(j*h),
                         w,
                         h);
        auto pos = ge->GetMousePos();
        if(pos.x > offs+(i*w) && pos.x < offs+(i*w) + w) {
            if(pos.y > offs+(j*h) && pos.y < offs+(j*h) + h) {
                ge->FillRect(offs+(i*w)+3,
                             offs+(j*h)+3,
                             w-5,
                             h-5,
                             olc::DARK_GREY);
            }
        }
        }
    }
}
