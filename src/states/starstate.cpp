#include "starstate.hpp"

#include "initstate.hpp"
#include "loadstate.hpp"

//todo remove
#include "components/components.hpp"
#include <cmath>

using namespace PlayingState;
StarState* StarState::m_state;

void StarState::init(TacticalGame* ge) {
    PRINT_FUNC

    pointofinterest = entt::null;
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
    auto tv = ge->get_tv();
    auto pos_mouse = ge->GetMousePos();
    if(ge->GetMouse(MOUSE_RBUTTON).bPressed) tv->StartPan(pos_mouse);
    if(ge->GetMouse(MOUSE_RBUTTON).bReleased) tv->EndPan(pos_mouse);
    if(ge->GetMouse(MOUSE_RBUTTON).bHeld) tv->UpdatePan(pos_mouse);

    if(ge->GetMouseWheel() < 0) tv->ZoomAtScreenPos(0.5f, pos_mouse);
    if(ge->GetMouseWheel() > 0) tv->ZoomAtScreenPos(2.0f, pos_mouse);


    if(ge->GetMouse(MOUSE_MBUTTON).bReleased) {
        // ge->change_state(TransitionState::LoadState::get());
        ge->push_state(InitState::get());
    }
    // if(ge->GetMouse(MOUSE_LBUTTON).bReleased) {
    // }
}
void StarState::update(TacticalGame* ge) {
    //PRINT_FUNC
    // std::cout << get_name() << " -> " << __func__ << "\t\t\t\r";
}

// TODO remake and move to utils
bool isInside(int circle_x, int circle_y,
              int rad, int x, int y)
{
    // Compare radius of circle with distance
    // of its center from given point
    if ((x - circle_x) * (x - circle_x) +
        (y - circle_y) * (y - circle_y) <= rad * rad)
        return true;
    else
        return false;
}

void draw_planets(TacticalGame* ge) {
    auto &reg = ge->get_reg();
    auto tv = ge->get_tv();

    auto get_name = [](){
        return "draw_planets";
    };


    for(auto [ent, pos, orb] : reg.view<Pos, Orbiting>().each())
    {
        auto new_angle = orb.angle + (orb.speed * ge->GetElapsedTime()) * 0.5f;
        if(new_angle > 360.f) new_angle -= 360.f;

        PRINT_TEXT_NOL(new_angle)
        auto rot_centre = ge->get<Pos>(orb.anchor).coordinates;

        float x = rot_centre.x - (orb.dist * cosf(new_angle));
        float y = rot_centre.y - (orb.dist * sinf(new_angle));

        pos.coordinates = {x, y};
        orb.angle = new_angle;

        tv->DrawLine(rot_centre, pos.coordinates, olc::CYAN);

        // tv->DrawCircle(pos.coordinates, circular.r, wireframe.color);
        // ++i;
    }
    auto mouse_pos = tv->ScaleToWorld(ge->GetMousePos()) + tv->GetWorldOffset();  // (ge->GetMousePos() - tv->GetTileOffset()) / tv->GetWorldScale();
    int i = 0;
    for(auto [ent, pos, wireframe, circular] : reg.view<Pos, Rendering::Wireframe, SizeCirc>().each())
    {
        tv->DrawCircle(pos.coordinates, circular.r, wireframe.color);
        ++i;
    }
}

void StarState::draw(TacticalGame* ge) {
    //PRINT_FUNC
    auto &reg = ge->get_reg();
    auto tv = ge->get_tv();

    static int offs = 50;
    static int w = 80;
    static int h = 50;

    draw_planets(ge);

    // int i = 0;
    // for(auto entity: ge->get_reg().view<entt::entity>()) {
    //     ge->DrawString({200,300*i}, "ENITY", olc::WHITE, 3);
    //     ++i;
    // }
    auto mouse_pos = tv->ScaleToWorld(ge->GetMousePos()) + tv->GetWorldOffset();  // (ge->GetMousePos() - tv->GetTileOffset()) / tv->GetWorldScale();
    entt::entity pressed = pointofinterest;
    for(auto [ent, pos, circular] : reg.view<Pos, SizeCirc, Tag::Hoverable, Tag::Selectable>().each()) {
        if(isInside(pos.coordinates.x, pos.coordinates.y, circular.r, mouse_pos.x, mouse_pos.y))
        {
            tv->FillCircle(pos.coordinates, circular.r*0.75, olc::WHITE);
            if(ge->GetMouse(MOUSE_LBUTTON).bReleased) {
                pressed = ent;
            }
        }
    }
    pointofinterest = pressed;

    if(pointofinterest != entt::null) {
        auto pos = ge->get<Pos>(pointofinterest);
        tv->SetWorldOffset(pos.coordinates - (tv->ScaleToWorld({ge->ScreenWidth()/2.f,ge->ScreenHeight()/2.f})));
    }

    // ge->get_tv()->FillCircle({ 0, 0 }, 20.0f, olc::RED);

    // for(auto i = 0; i < 16; ++i) {
    //     for(auto j = 0; j < 16; ++j) {
    //         ge->get_tv()->DrawRect(offs+(i*w),
    //                      offs+(j*h),
    //                      w,
    //                      h);
    //         auto pos = get_mouse_pos_screen(ge); // ge->get_tv()->ScaleToWorld(ge->GetMousePos()) + ge->get_tv()->GetWorldOffset();  // (ge->GetMousePos() - ge->get_tv()->GetTileOffset()) / ge->get_tv()->GetWorldScale();
    //         if(pos.x > offs+(i*w) && pos.x < offs+(i*w) + w) {
    //             if(pos.y > offs+(j*h) && pos.y < offs+(j*h) + h) {
    //                 ge->get_tv()->FillRect(offs+(i*w)+3,
    //                                 offs+(j*h)+3,
    //                                 w-5,
    //                                 h-5,
    //                                 olc::DARK_GREY);
    //             }
    //         }
    //     }
    // }
}
