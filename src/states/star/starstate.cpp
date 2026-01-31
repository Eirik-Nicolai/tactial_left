#include "starstate.hpp"

#include "states/init/initstate.hpp"
#include "states/load/loadstate.hpp"
#include "systems/rendering.hpp"

#include "utils/ecs.hpp"
//todo remove
#include "components/components.hpp"
#include <cmath>

#include "logger.hpp"

#include "engine/input.hpp"

using namespace PlayingState;
StarState* StarState::m_state;


class TestInput : public Input {
    public:
    std::string get_name() const final { return "Test Input StarState key P"; };
    void execute(TacticalGame *ge) final {
        Error("Triggered input {}", get_name());
    }
};

class PanInputStart : public Input {
    public:
    std::string get_name() const final { return "InputPanningStart"; };
    void execute(TacticalGame *ge) final {
        auto tv = ge->get_tv();
        auto pos_mouse = ge->GetMousePos();
        ge->get_tv()->StartPan(pos_mouse);
    }
};

class ScrollUpInput : public Input {
    public:
    std::string get_name() const final { return "ScrollUpInputStart"; };
    void execute(TacticalGame *ge) final {
        auto tv = ge->get_tv();
        auto pos_mouse = ge->GetMousePos();
        tv->ZoomAtScreenPos(0.5f, pos_mouse);
    }
};

class ScrollDownInput : public Input {
    public:
    std::string get_name() const final { return "ScrollDownInputStart"; };
    void execute(TacticalGame *ge) final {
        auto tv = ge->get_tv();
        auto pos_mouse = ge->GetMousePos();
        tv->ZoomAtScreenPos(2.f, pos_mouse);
    }
};

class PanInputEnd : public Input {
    public:
    std::string get_name() const final { return "InputPanningEnd"; };
    void execute(TacticalGame *ge) final {
        auto tv = ge->get_tv();
        auto pos_mouse = ge->GetMousePos();
        ge->get_tv()->EndPan(pos_mouse);
    }
};

class PanInputUpdate : public Input {
    public:
    std::string get_name() const final { return "InputPanningUpdate"; };
    void execute(TacticalGame *ge) final {
        auto tv = ge->get_tv();
        auto pos_mouse = ge->GetMousePos();
        ge->get_tv()->UpdatePan(pos_mouse);
    }
};

void StarState::init(TacticalGame* ge) {
    LOG_FUNC
    handler = std::make_unique<InputHandler>();

    handler->register_input(INPUT_TYPE::middle_mouse_PRESSED,
                            std::make_shared<InputDebugWrapper>(
                                std::make_shared<PanInputStart>()));
    handler->register_input(INPUT_TYPE::middle_mouse_RELEASED,
                            std::make_shared<InputDebugWrapper>(
                                std::make_shared<PanInputEnd>()));
    handler->register_input(INPUT_TYPE::middle_mouse_HELD,
                            std::make_shared<InputDebugWrapper>(
                                std::make_shared<PanInputUpdate>()));

    handler->register_input(INPUT_TYPE::middle_mouse_SCROLLDOWN,
                            std::make_shared<InputDebugWrapper>(
                                std::make_shared<ScrollUpInput>()));

    handler->register_input(INPUT_TYPE::middle_mouse_SCROLLUP,
                            std::make_shared<InputDebugWrapper>(
                                std::make_shared<ScrollDownInput>()));



    handler->register_input(INPUT_TYPE::left_mouse_PRESSED,
                            std::make_shared<TestInput>());
}
void StarState::cleanup(TacticalGame* ge) {
    LOG_FUNC
}

void StarState::enter(TacticalGame* ge) {
    LOG_FUNC
}
void StarState::exit(TacticalGame* ge) {
    LOG_FUNC
}

void StarState::pause(TacticalGame* ge) {
    LOG_FUNC

}
void StarState::resume(TacticalGame* ge) {
    LOG_FUNC

}
void StarState::handle_input(TacticalGame* ge) {
    auto input = handler->get_input(ge);
    input->execute(ge);


    // //LOG_FUNC
    // auto tv = ge->get_tv();
    // auto pos_mouse = ge->GetMousePos();

    // if(ge->GetMouse(MOUSE_RBUTTON).bReleased) {
    //     // ge->change_state(TransitionState::LoadState::get());
    //     ge->push_state(InitState::Instance());
    // }

    // if(ge->GetMouse(MOUSE_LBUTTON).bReleased) {
    //     auto &reg = ge->get_reg();
    //     auto mouse_pos = tv->ScaleToWorld(pos_mouse) + tv->GetWorldOffset();  // (ge->GetMousePos() - tv->GetTileOffset()) / tv->GetWorldScale();
    //     for(auto [ent, pos, circular] : reg.view<Pos, SizeCirc, Tag::Hoverable, Tag::Selectable>().each()) {
    //         if(isInside(pos.coordinates.x, pos.coordinates.y, circular.r, mouse_pos.x, mouse_pos.y))
    //         {
    //             tv->FillCircle(pos.coordinates, circular.r*0.75, olc::WHITE);
    //             if(ge->GetMouse(MOUSE_LBUTTON).bReleased) {
    //                 StarStateSelected::Instance()->set_camera_point(ent);
    //                 ge->push_state(StarStateSelected::Instance());
    //             }
    //         }
    //     }
    // }
}
void StarState::update(TacticalGame* ge) {
    //LOG_FUNC
    // std::cout << get_name() << " -> " << __func__ << "\t\t\t\r";
    auto &reg = ge->get_reg();

    auto tv = ge->get_tv();
    auto pos_mouse = ge->GetMousePos();

    for(auto [ent, pos, orb] : reg.view<Pos, Orbiting>().each())
    {
        auto new_angle = orb.angle + (orb.speed * ge->GetElapsedTime()) * 0.5f;
        if(new_angle > 360.f) new_angle -= 360.f;

        //Debug(new_angle);
        auto rot_centre = get<Pos>(reg, orb.anchor);

        float x = rot_centre.x - (orb.dist * cosf(new_angle));
        float y = rot_centre.y - (orb.dist * sinf(new_angle));

        pos.x = x;
        pos.y = y;
        orb.angle = new_angle;

        // tv->DrawLine(rot_centre.x, rot_centre.y, pos.x, pos.y, olc::CYAN, 0xF0F0F0F0);
    }

    auto mouse_pos = tv->ScaleToWorld(pos_mouse) + tv->GetWorldOffset();  // (ge->GetMousePos() - tv->GetTileOffset()) / tv->GetWorldScale();
    for(auto [ent, pos, size] : reg.view<Pos, Rendering::Size>().each()) {
        if(isInside(pos.x, pos.y, size.h, mouse_pos.x, mouse_pos.y))
        {
            Debug("INSIDE ENTITY {} {}", Debugging::entity_name(reg, ent), Debugging::entity_id(reg, ent));
            if(!has<_hovered>(reg, ent)) reg.emplace<_hovered>(ent);
        } else {
            if(has<_hovered>(reg,ent)) reg.remove<_hovered>(ent);
        }
    }
}

void StarState::draw(TacticalGame* ge) {
    //LOG_FUNC
    auto &reg = ge->get_reg();


}


StarStateSelected* StarStateSelected::m_state;

void StarStateSelected::init(TacticalGame* ge) {
    LOG_FUNC

    pointofinterest = entt::null;
}
void StarStateSelected::cleanup(TacticalGame* ge) {
    LOG_FUNC
}

void StarStateSelected::enter(TacticalGame* ge) {
    LOG_FUNC

    auto tv = ge->get_tv();
    tv->SetWorldScale({3.f, 3.f});
}
void StarStateSelected::exit(TacticalGame* ge) {
    LOG_FUNC

    auto tv = ge->get_tv();
    tv->SetWorldScale({1.f, 1.f});
}

void StarStateSelected::pause(TacticalGame* ge) {
    LOG_FUNC

}
void StarStateSelected::resume(TacticalGame* ge) {
    LOG_FUNC

}
void StarStateSelected::handle_input(TacticalGame* ge) {
    //LOG_FUNC
    if(ge->GetMouse(MOUSE_MBUTTON).bReleased) {
        pointofinterest = entt::null;
        ge->pop_state();
    }
    if(ge->GetMouse(MOUSE_LBUTTON).bReleased) {
        pointofinterest = entt::null;
        ge->change_state(TransitionState::LoadState::Instance());
    }
}
void StarStateSelected::update(TacticalGame* ge) {
    StarState::update(ge);
    if(pointofinterest != entt::null) {
        auto tv = ge->get_tv();
        auto pos = get<Pos>(ge->get_reg(),pointofinterest);
        auto coordinates = olc::vi2d(pos.x, pos.y);
        tv->SetWorldOffset(coordinates - (tv->ScaleToWorld({ge->ScreenWidth()/2.f,ge->ScreenHeight()/2.f})));
    }
}
void StarStateSelected::draw(TacticalGame* ge) {
    //LOG_FUNC
    auto &reg = ge->get_reg();

    ge->DrawRect({0,0}, ge->GetScreenSize()-1);
    ge->DrawString(200, 200, "This is a string in selected state", olc::BLUE, 3);
}
