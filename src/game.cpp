#define OLC_PGE_APPLICATION
#include "game.hpp"

void InitState::handle_input() {

}
void InitState::update() {


}
void InitState::draw(TacticalGame* ge) {
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

TacticalGame::TacticalGame()
{
    sAppName = "TACTICAL LEFTIST";
}

bool TacticalGame::OnUserCreate()
{
    CURR_STATE = std::unique_ptr<InitState>();
    if(CURR_STATE) {
        std::cout << "LOADED STATE" << std::endl;
    }
    m_fElapsedTimeSinceTick = 0;

    return true;
}

bool TacticalGame::OnUserUpdate(float dt)
{
    Clear(olc::BLACK);


    // CURR_STATE = NEXT_STATE;

    // CURR_STATE->handle_input();
    // CURR_STATE->update();

    if(CURR_STATE) {
        CURR_STATE->draw(this);
    }

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
