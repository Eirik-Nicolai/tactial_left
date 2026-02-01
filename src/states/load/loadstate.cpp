#include "loadstate.hpp"

#include "states/init/initstate.hpp"
#include "states/star/starstate.hpp"

#include "components/components.hpp"
#include "logger.hpp"

#include <thread>
#include <cstdlib>

#define LOADING_ELEMENTS 3

using namespace TransitionState;
LoadState* LoadState::m_state;

unsigned bounded_rand(unsigned upper_range = 2)
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
    for(auto i = 0; i < LOADING_ELEMENTS; ++i) {
        Trace("Adding element");
        assets.emplace_back(i);
        sleep_for(bounded_rand());
    }
    Debug("Finished thread");
}

void LoadState::init(TacticalGame* ge) {
    LOG_FUNC
    olc::Sprite* s;

    olc::Decal d = olc::Decal(s);

    ge->SetDrawTarget(1);
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
    reg.emplace<Pos>(sun, 0, 0);
    reg.emplace<Rendering::Wireframe>(sun, Rendering::Wireframe::TYPE::CIRCLE_FILL, olc::DARK_RED);
    reg.emplace<Size>(sun, 60, 0);

    auto earth = reg.create();
    reg.emplace<Pos>(earth, 50, 50 );
    reg.emplace<Rendering::Wireframe>(earth, Rendering::Wireframe::TYPE::TRIANGLE_FILL, olc::DARK_BLUE);
    reg.emplace<Size>(earth, 30, 0);
    reg.emplace<_selectable>(earth);
    reg.emplace<_hoverable>(earth);

    auto moon = reg.create();
    reg.emplace<Pos>(moon, 100, 100 );
    reg.emplace<Rendering::Wireframe>(moon,Rendering::Wireframe::TYPE::SQUARE, olc::WHITE);
    reg.emplace<Size>(moon, 10, 10);
    reg.emplace<_selectable>(moon);
    reg.emplace<_hoverable>(moon);


    reg.emplace<Orbiting>(earth, sun, 500.f, 0.1f);
    reg.emplace<Orbiting>(moon, earth, 50.f, -0.5f);

    reg.emplace<Debugging::Debug>(sun, "SUN");
    reg.emplace<Debugging::Debug>(earth, "EARTH");
    reg.emplace<Debugging::Debug>(moon, "MOON");
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

    if(m_assets.size() >= LOADING_ELEMENTS) {
        sElapsedTime += ge->GetElapsedTime();
    }

    if(sElapsedTime > 1) { // 3 second wait to simulate loading
        ge->change_state(PlayingState::StarState::Instance());
    }
}

void LoadState::draw(TacticalGame* ge) {
    //LOG_FUNC

    if(m_assets.size() >= LOADING_ELEMENTS) {
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
