#include "loadstate.hpp"

#include "states/init/initstate.hpp"
#include "states/star/starstate.hpp"

#include "components/components.hpp"
#include "logger.hpp"

using namespace TransitionState;
LoadState* LoadState::m_state;

void LoadState::init(TacticalGame* ge) {
    TRACE_LOG_FUNC
}

void LoadState::cleanup(TacticalGame* ge) {
    TRACE_LOG_FUNC
}

void LoadState::enter(TacticalGame* ge) {
    TRACE_LOG_FUNC
    sElapsedTime = 0;

    auto& reg = ge->get_reg();
    auto sun = reg.create();
    reg.emplace<Pos>(sun, olc::vf2d(0, 0 ));
    reg.emplace<Rendering::Wireframe>(sun, olc::DARK_RED);
    reg.emplace<SizeCirc>(sun, 60);

    auto earth = reg.create();
    reg.emplace<Pos>(earth, olc::vf2d(0, 0 ));
    reg.emplace<Rendering::Wireframe>(earth, olc::DARK_BLUE);
    reg.emplace<SizeCirc>(earth, 10);
    reg.emplace<Tag::Selectable>(earth);
    reg.emplace<Tag::Hoverable>(earth);

    auto moon = reg.create();
    reg.emplace<Pos>(moon, olc::vf2d(0, 0 ));
    reg.emplace<Rendering::Wireframe>(moon, olc::DARK_GREY);
    reg.emplace<SizeCirc>(moon, 5);
    reg.emplace<Tag::Selectable>(moon);
    reg.emplace<Tag::Hoverable>(moon);


    reg.emplace<Orbiting>(earth, sun, 500.f, 0.1f);
    reg.emplace<Orbiting>(moon, earth, 50.f, -0.5f);
}
void LoadState::exit(TacticalGame* ge) {
    TRACE_LOG_FUNC
}

void LoadState::pause(TacticalGame* ge) {
    TRACE_LOG_FUNC

}
void LoadState::resume(TacticalGame* ge) {
    TRACE_LOG_FUNC

}
void LoadState::handle_input(TacticalGame* ge) {
    //TRACE_LOG_FUNC

    if(ge->GetMouse(MOUSE_LBUTTON).bReleased) {
        TRACE_LOG_TEXT("Loading, please wait")
    }
}
void LoadState::update(TacticalGame* ge) {
    TRACE_LOG_TEXT_NOL(sElapsedTime)

    sElapsedTime += ge->GetElapsedTime();

    if(sElapsedTime > 1) { // 3 second wait to simulate loading
        ge->change_state(PlayingState::StarState::Instance());
    }
}

void LoadState::draw(TacticalGame* ge) {
    //TRACE_LOG_FUNC

    ge->DrawString(300, 300, "LOADING, PLEASE WAIT...",olc::RED, 3);
}
