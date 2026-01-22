#define OLC_PGE_APPLICATION
#include "game.hpp"

TacticalGame::TacticalGame()
{
    sAppName = "TACTICAL LEFTIST";
}

bool TacticalGame::OnUserCreate()
{
    CURR_STATE = {state::PAUSED, type::INIT};
    NEXT_STATE = CURR_STATE;

    m_fElapsedTimeSinceTick = 0;

    return true;
}

bool TacticalGame::OnUserUpdate(float dt)
{
    Clear(olc::BLACK);

    std::cout << "tick" << std::endl;
    // Debugger::instance().Clear();

    // switch(CURR_STATE.state)
    // {
    //     case state::COMBAT:
    //     {
    //         STATE_COMBAT(dt);
    //     }
    //     break;

    //     case state::PAUSED:
    //     {
    //         STATE_PAUSE(dt);
    //     }
    //     break;

    //     case state::WALKING:
    //     {
    //         STATE_WALKING(dt);
    //     }
    //     break;

    //     default:
    //     {
    //         std::cout << "not impl" << std::endl;
    //     }
    // }

    // Debugger::instance().Print(this, 0.80, 0.80);
    CURR_STATE = NEXT_STATE;

    return true;
}


// bool TacticalGame::delay_for(float delay, float dt)
// {
//     if(m_elapsed_transition_time >= delay)
//     {
//         m_elapsed_transition_time = 0;
//         return true;
//     }
//     m_elapsed_transition_time+= dt;
//     return false;
// }

// std::string TacticalGame::get_name(const entt::entity &ent, std::string ret)
// {
//     visual v;
//     if(tryget_component(ent, v))
//     {
//         return v.name;
//     }
//     return ret;
// }
