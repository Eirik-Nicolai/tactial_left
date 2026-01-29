#define OLC_PGE_APPLICATION
#include "olc/olcPixelGameEngine.h"

#define OLC_PGEX_TRANSFORMEDVIEW
#include "olc/olcPGEX_TransformedView.h"

#include "game.hpp"
#include "states/init/initstate.hpp"
#include "states/star/starstate.hpp"
#include "states/load/loadstate.hpp"
#include "states/combat/combatstate.hpp"

#include "systems/rendering.hpp"


//TODO move to own file/class
#include <fstream>
#include <fkYAML/node.hpp>


void TacticalGame::push_state(GameState* state) {
    LOG_FUNC
    Debug("Pushing state {}", state->get_name());
    if(!m_states.empty()) {
        Debug("Pausing state {}", m_states.back()->get_name());
        m_states.back()->pause(this);
    }

    m_states.push_back(state);
    m_states.back()->enter(this);
}
void TacticalGame::pop_state() {
    LOG_FUNC
    if(!m_states.empty()) {
        Debug("List before {}", m_states.size());
        m_states.back()->exit(this);
        m_states.pop_back();
    }

    Debug("List after {}", m_states.size());
    if(!m_states.empty()) {
        m_states.back()->resume(this);
    }
}
void TacticalGame::change_state(GameState* state) {
    LOG_FUNC
    // cleanup all the current states
    Debug("Cleaning up {} states", m_states.size());
    while ( !m_states.empty() ) {
        m_states.back()->exit(this);
        Debug("Exiting state {}", m_states.back()->get_name());
        m_states.pop_back();
    }

    Debug("Changing to state {}", state->get_name());
    // store and init the new state
    m_states.push_back(state);
    m_states.back()->enter(this);
}

TacticalGame::TacticalGame()
{
    sAppName = "TACTICAL LEFTIST";

    m_system_managers_amount = 0;
}

bool TacticalGame::OnUserDestroy() {
    PlayingState::InitState::Instance()->cleanup(this);
    PlayingState::CombatState::Instance()->cleanup(this);
    PlayingState::StarState::Instance()->cleanup(this);
    TransitionState::LoadState::Instance()->cleanup(this);

    return true;
}

bool TacticalGame::OnUserCreate()
{
    std::cout << "Initializing game ..." << std::endl;;

    std::cout << "READ CONFIG.." << std::endl;

    // TODO move all this logic to config reading
    // class
    std::fstream fs;
    fs.open("/home/enoo/repos/personal/tactical_leftist/config.yml");
    if(!fs.is_open()) {
        //Error("NO CONFIG FILE");
        throw std::runtime_error("Unable to find config");
    }

    auto conf = fkyaml::node::deserialize(fs);
    try {
        // TODO can cast string to enum instead
        auto conf_level = [&](std::string& conf) -> int {
            if(conf == "TRACE") {
                    return 0;
            }
            if(conf == "DEBUG") {
                    return 1;
            }
            if(conf == "INFO") {
                    return 2;
            }
            throw std::runtime_error("Throw");
        };

        auto level = conf.at("log_level").as_str();
        std::cout << "Level is " << level << std::endl;
        Logger::Get()->set_log_level(
            static_cast<spdlog::level::level_enum>(conf_level(level)));
    } catch(...) {
        throw std::runtime_error("CANNOT READ CONFIG");
    }

    // TODO maybe change these to be
    // stored in a list inside the GE ?
    Debug("Initializing game states");
    PlayingState::InitState::Instance()->init(this);
    PlayingState::StarState::Instance()->init(this);
    PlayingState::CombatState::Instance()->init(this);
    TransitionState::LoadState::Instance()->init(this);

    Debug("Setting up camera");
    tvp = std::make_shared<olc::TileTransformedView>(
        olc::vi2d( ScreenWidth(), ScreenHeight()),
        olc::vi2d(1, 1)
    );

    tvp->SetWorldScale({1.0f, 1.0f});
    tvp->SetWorldOffset(olc::vi2d(0.f, 0.f) - (tvp->ScaleToWorld({ScreenWidth()/2.f,ScreenHeight()/2.f})));

    Debug("Setting init state");
    change_state(TransitionState::LoadState::Instance());
    if(!m_states.empty()) {
        Info("Starting on state {}", m_states.front()->get_name());
    }

    Debug("Initiating systems");
    // TODO add helper function
    auto rendering_manager = std::make_unique<RenderingSystemManager>();
    rendering_manager->add(std::make_unique<PreRenderer>());
    rendering_manager->add(std::make_unique<FirstRenderer>());
    rendering_manager->add(std::make_unique<SecondRenderer>());
    rendering_manager->add(std::make_unique<ThirdRenderer>());
    rendering_manager->add(std::make_unique<GUIRenderer>());
    rendering_manager->add(std::make_unique<WireframeRenderer>());
    Debug("Adding {}", rendering_manager->get_name());
    m_system_managers[m_system_managers_amount] = std::move(rendering_manager);
    m_system_managers_amount++;
    return true;
}

bool TacticalGame::OnUserUpdate(float dt)
{
    auto CURR_STATE = m_states.back();

    // HACK shoud find a better way of testing state
    // but also shouldnt really need it
    if(GetKey(olc::Key::P).bReleased && CURR_STATE->get_name()!="InitState") {
        push_state(PlayingState::InitState::Instance());
    }

    if(CURR_STATE) {
        CURR_STATE->handle_input(this);
        CURR_STATE->update(this);
    }
    for(auto &state : m_states) {
        //state->draw(this);
    }


    for(int i = 0; i < m_system_managers_amount; ++i) {
        m_system_managers[i]->dispatch(this);
    }
    return true;
}
