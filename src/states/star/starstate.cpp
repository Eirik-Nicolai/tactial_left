#include "starstate.hpp"

#include "states/init/initstate.hpp"
#include "states/load/loadstate.hpp"
#include "systems/rendering.hpp"

#include "utils/ecs.hpp"
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
    if(pointofinterest == entt::null) {
        if(ge->GetMouse(MOUSE_MBUTTON).bPressed) tv->StartPan(pos_mouse);
        if(ge->GetMouse(MOUSE_MBUTTON).bReleased) tv->EndPan(pos_mouse);
        if(ge->GetMouse(MOUSE_MBUTTON).bHeld) tv->UpdatePan(pos_mouse);

        if(ge->GetMouseWheel() < 0) tv->ZoomAtScreenPos(0.5f, pos_mouse);
        if(ge->GetMouseWheel() > 0) tv->ZoomAtScreenPos(2.0f, pos_mouse);
    }

    if(ge->GetMouse(MOUSE_RBUTTON).bReleased) {
        // ge->change_state(TransitionState::LoadState::get());
        ge->push_state(InitState::Instance());
    }

    if(ge->GetMouse(MOUSE_LBUTTON).bReleased) {
        auto &reg = ge->get_reg();
        auto mouse_pos = tv->ScaleToWorld(pos_mouse) + tv->GetWorldOffset();  // (ge->GetMousePos() - tv->GetTileOffset()) / tv->GetWorldScale();
        for(auto [ent, pos, circular] : reg.view<Pos, SizeCirc, Tag::Hoverable, Tag::Selectable>().each()) {
            if(isInside(pos.coordinates.x, pos.coordinates.y, circular.r, mouse_pos.x, mouse_pos.y))
            {
                tv->FillCircle(pos.coordinates, circular.r*0.75, olc::WHITE);
                if(ge->GetMouse(MOUSE_LBUTTON).bReleased) {
                    pointofinterest = ent;
                }
            } else {
                pointofinterest = entt::null;
            }
        }
    }
}
void StarState::update(TacticalGame* ge) {
    //PRINT_FUNC
    // std::cout << get_name() << " -> " << __func__ << "\t\t\t\r";
    auto &reg = ge->get_reg();

    auto tv = ge->get_tv();
    auto pos_mouse = ge->GetMousePos();

    for(auto [ent, pos, orb] : reg.view<Pos, Orbiting>().each())
    {
        auto new_angle = orb.angle + (orb.speed * ge->GetElapsedTime()) * 0.5f;
        if(new_angle > 360.f) new_angle -= 360.f;

        PRINT_TEXT_NOL(new_angle)
        auto rot_centre = get<Pos>(reg, orb.anchor).coordinates;

        float x = rot_centre.x - (orb.dist * cosf(new_angle));
        float y = rot_centre.y - (orb.dist * sinf(new_angle));

        pos.coordinates = {x, y};
        orb.angle = new_angle;

        tv->DrawLine(rot_centre, pos.coordinates, olc::CYAN, 0xF0F0F0F0);
    }

    if(pointofinterest != entt::null) {
        auto tv = ge->get_tv();
        auto pos = get<Pos>(reg,pointofinterest);
        tv->SetWorldOffset(pos.coordinates - (tv->ScaleToWorld({ge->ScreenWidth()/2.f,ge->ScreenHeight()/2.f})));
    }

    auto mouse_pos = tv->ScaleToWorld(pos_mouse) + tv->GetWorldOffset();  // (ge->GetMousePos() - tv->GetTileOffset()) / tv->GetWorldScale();
    for(auto [ent, pos, circular] : reg.view<Pos, SizeCirc, Tag::Hoverable, Tag::Selectable>().each()) {
        if(isInside(pos.coordinates.x, pos.coordinates.y, circular.r, mouse_pos.x, mouse_pos.y))
        {
            if(!has<Tag::Hovered>(reg, ent)) reg.emplace<Tag::Hovered>(ent);
        } else {
            if(has<Tag::Hovered>(reg,ent)) reg.remove<Tag::Hovered>(ent);
        }
    }
}

void StarState::draw(TacticalGame* ge) {
    //PRINT_FUNC
    auto &reg = ge->get_reg();

    State::Star::render_stars(reg, ge);
}
