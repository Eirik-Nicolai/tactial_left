#include "loadstate.hpp"

#include "states/init/initstate.hpp"
#include "states/star/starstate.hpp"

#include "components/components.hpp"
#include "logger.hpp"

#include <thread>
#include <cstdlib>

using namespace TransitionState;
LoadState* LoadState::m_state;

unsigned bounded_rand(unsigned upper_range = 6)
{
    for (unsigned x, r;;)
        if (x = rand(), r = x % upper_range, x - r <= -upper_range)
            return r;
}

void sleep_for(int seconds) {
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
}

void do_work(std::vector<int>& assets) {
    sleep_for(bounded_rand());
    Trace("Adding element");
    assets.emplace_back(5);
    sleep_for(bounded_rand());
    Trace("Adding element");
    assets.emplace_back(5);
    sleep_for(bounded_rand());
    Trace("Adding element");
    assets.emplace_back(5);
    sleep_for(bounded_rand());
    Trace("Adding element");
    assets.emplace_back(5);
    sleep_for(bounded_rand());
    Debug("Finished thread");
}

void LoadState::init(TacticalGame* ge) {
    LOG_FUNC
}

void LoadState::cleanup(TacticalGame* ge) {
    LOG_FUNC
}

void LoadState::enter(TacticalGame* ge) {
    LOG_FUNC
    sElapsedTime = 0;
    m_assets.clear();


    Debug("Making thread");
    m_tLoader = std::thread(do_work, std::ref(m_assets));
    m_tLoader.detach();

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


    m_assets.emplace_back(1);

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
    LOG_FUNC
}

void LoadState::pause(TacticalGame* ge) {
    LOG_FUNC

}
void LoadState::resume(TacticalGame* ge) {
    LOG_FUNC

}
void LoadState::handle_input(TacticalGame* ge) {
    //LOG_FUNC

    if(ge->GetMouse(MOUSE_LBUTTON).bReleased) {
        //TRACE_LOG_TEXT("Loading, please wait")
    }
}
void LoadState::update(TacticalGame* ge) {
    //TRACE_LOG_TEXT_NOL(sElapsedTime)

    if(m_assets.size() >= 5) {
        sElapsedTime += ge->GetElapsedTime();
    }

    if(sElapsedTime > 1) { // 3 second wait to simulate loading
        ge->change_state(PlayingState::StarState::Instance());
    }
}

void LoadState::draw(TacticalGame* ge) {
    //LOG_FUNC

    if(m_assets.size() >= 5) {
        ge->DrawString(400, 300, "DONE",olc::RED, 3);
    } else {
        ge->DrawString(400, 300, "LOADING, PLEASE WAIT...",olc::RED, 3);
    }


    int width = 1200;
    float factor = (m_assets.size()/5.f);

    ge->FillRect(200, 500, (width*factor), 50, olc::RED);
    ge->DrawRect(200, 500, width, 50);

    ge->DrawString({20,30}, std::to_string(factor));
    ge->DrawString({20,50}, std::to_string(m_assets.size()));
}
