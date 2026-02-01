#include "rendering.hpp"

#include "components/components.hpp"
#include "utils/geometry.hpp"
#include "states/star/starstate.hpp"
#include "logger.hpp"

#include "utils/ecs.hpp"

// void draw_planets(TacticalGame* ge) {
//     auto &reg = ge->get_reg();
//     auto tv = ge->get_tv();

//     auto get_name = [](){
//         return "draw_planets";
//     };

//     for(auto [ent, pos, orb] : reg.view<Pos, Orbiting>().each())
//     {
//         auto rot_centre = get<Pos>(reg, orb.anchor).coordinates;

//         tv->DrawLine(rot_centre, pos.coordinates, olc::CYAN, 0xF0F0F0F0);
//     }

//     for(auto [ent, pos, wireframe, circular] : reg.view<Pos, Rendering::Wireframe, SizeCirc>().each())
//     {
//         tv->DrawCircle(pos.coordinates, circular.r, wireframe.color);
//     }
// }

// void State::Star::render_stars(entt::registry &reg, TacticalGame* ge) {

//     static int offs = 50;
//     static int w = 80;
//     static int h = 50;

//     draw_planets(ge);

//     for(auto [ent, pos, circular] : reg.view<Pos, SizeCirc, Tag::Hovered>().each()) {
//         auto tv = ge->get_tv();
//         tv->FillCircle(pos.coordinates, circular.r*0.75, olc::WHITE);
//     }
// }

// TODO move all this to arent class init since we don't change it between
// types of managers
// or make templated ? unsure what is better
RenderingSystemManager::RenderingSystemManager() {
    Debug("{} initialized", get_name());
    m_systems = std::array<std::unique_ptr<System>, MAX_SYSTEM_AMOUNT>();
    m_system_amount = 0;
}

void RenderingSystemManager::add(std::unique_ptr<System> system) {
    Debug("Adding to list of systems, size: {}", m_system_amount);
    if(m_system_amount < m_systems.max_size()) {
        m_systems[m_system_amount] = std::move(system);
        m_system_amount++;
    } else {
        Error("System manager {} is full !", get_name());
    }
}

void RenderingSystemManager::dispatch(TacticalGame* ge) {
    for(int i = 0; i < m_system_amount; ++i) {
        m_systems[i]->execute(ge);
    }
}

// ---------------------------
// | FIXME TODO
// | OLC PGE doesnt batch sprites for drawcall and instead uses one per sprite
// | update this
// ---------------------------

void PreRenderer::execute(TacticalGame* ge) {
    LOG_FUNC
    ge->Clear(olc::BLACK);
    // any other init rendering step, might not be needed
}

void WireframeRenderer::execute(TacticalGame* ge) {
    LOG_FUNC
    auto& reg = ge->get_reg();
    auto tv = ge->get_tv();

    // for(auto [ent, pos, size, wireframe] : reg.view<Pos, Rendering::Size, Rendering::Wireframe>().each()) {
    //     auto tv = ge->get_tv();
    //     switch (wireframe.type) {
    //         case Rendering::Wireframe::TYPE::CIRCLE: {
    //             tv->DrawCircle(pos.x, pos.y, size.h, wireframe.color);
    //         } break;
    //         case Rendering::Wireframe::TYPE::CIRCLE_FILL: {
    //             tv->FillCircle(pos.x, pos.y, size.h, wireframe.color);
    //         } break;
    //         case Rendering::Wireframe::TYPE::SQUARE: {
    //             tv->DrawRect(pos.x, pos.y, size.w, size.h, wireframe.color);
    //         } break;
    //         case Rendering::Wireframe::TYPE::SQUARE_FILL: {
    //             tv->FillRect(pos.x, pos.y, size.w, size.h, wireframe.color);
    //         } break;
    //         case Rendering::Wireframe::TYPE::TRIANGLE: {
    //             auto side_opposite = (int) (size.h/sqrt(3));
    //             olc::vi2d pos1 = {pos.x, pos.y};
    //             olc::vi2d pos2 = {pos.x+size.h - side_opposite, pos.y+size.h};
    //             olc::vi2d pos3 = {pos.x+size.h + side_opposite, pos.y+size.h};
    //             tv->DrawTriangle(pos1, pos2, pos3);
    //         } break;
    //         case Rendering::Wireframe::TYPE::TRIANGLE_FILL: {
    //             auto side_opposite = (int) (size.h/sqrt(3));
    //             olc::vi2d pos1 = {pos.x, pos.y};
    //             olc::vi2d pos2 = {pos.x + side_opposite, pos.y+size.h};
    //             olc::vi2d pos3 = {pos.x - side_opposite, pos.y+size.h};
    //             tv->FillTriangle(pos1, pos2, pos3);
    //         } break;
    //         default:
    //             Error("Entity does not have a valid type {}", (int)wireframe.type);

    //     };
    // }
}


void FirstRenderer::execute(TacticalGame* ge) {
    LOG_FUNC

    ge->SetDrawTarget(ge->layer_1.get());

    ge->DrawString(10,10,"HELLO FROM 1", olc::VERY_DARK_RED, 2);

    ge->get_tv()->FillCircle(ge->test_x, 100, 40, olc::BLUE);
    if(ge->fElapsedTime >= 0.5) {
        ge->fElapsedTime=0;
        ge->test_x+=10;
    }
    ge->fElapsedTime += ge->GetElapsedTime();
}


void SecondRenderer::execute(TacticalGame* ge) {
    LOG_FUNC

    ge->SetDrawTarget(ge->layer_2.get());

    ge->DrawRect(120, 120, 30, 30);
    ge->get_tv()->FillRect(120, 120, 30, 30);

    ge->DrawString(14,14,"HELLO FROM 2", olc::DARK_RED, 2);

    ge->get_tv()->FillCircle(ge->test_x+10, 100, 40, olc::DARK_BLUE);
}


void ThirdRenderer::execute(TacticalGame* ge) {
    LOG_FUNC

    ge->SetDrawTarget(ge->layer_3.get());

    ge->DrawString(18,18,"HELLO FROM 3", olc::RED, 2);

    for(auto i = 0; i < 1000; ++i) {
        ge->FillCircle(30, 10+(i*3), 40, olc::GREEN);
    }
    ge->get_tv()->FillCircle(ge->test_x+21, 100, 40, olc::VERY_DARK_BLUE);
}


void PostRenderer::execute(TacticalGame* ge) {
    LOG_FUNC

}


void GUIRenderer::execute(TacticalGame* ge) {
    LOG_FUNC
    auto& reg = ge->get_reg();
    auto tv = ge->get_tv();

    // for(auto [ent, pos, size, wireframe] : reg.view<Pos, Rendering::Size, Rendering::Wireframe>().each())
    // {
    //     tv->DrawCircle(pos.x, pos.y, size.h, wireframe.color);
    // }
}
