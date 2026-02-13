#include "rendering.hpp"

#include "components/gui.hpp"
#include "components/rendering.hpp"
#include "utils/geometry.hpp"
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

// // TODO move all this to arent class init since we don't change it between
// // types of managers
// // or make templated ? unsure what is better
// RenderingSystemManager::RenderingSystemManager() {
//     Debug("{} initialized", get_name());
//     m_systems = std::array<std::unique_ptr<System>, MAX_SYSTEM_AMOUNT>();
//     m_system_amount = 0;
// }

// void RenderingSystemManager::add(std::unique_ptr<System> system) {
//     Debug("Adding to list of systems, size: {}", m_system_amount);
//     if(m_system_amount < m_systems.max_size()) {
//         m_systems[m_system_amount] = std::move(system);
//         m_system_amount++;
//     } else {
//         Error("System manager {} is full !", get_name());
//     }
// }

// void RenderingSystemManager::dispatch(TacticalGame* ge) {
//     for(int i = 0; i < m_system_amount; ++i) {
//         m_systems[i]->execute(ge);
//     }
// }

// ---------------------------
// | FIXME TODO
// | OLC PGE doesnt batch sprites for drawcall and instead uses one per sprite
// | update this
// ---------------------------

void PreRenderer::execute(TacticalGame *ge) {
  LOG_FUNC
  // ge->Clear(olc::BLACK);
  // any other init rendering step, might not be needed
}

void WireframeRenderer::execute(TacticalGame* ge) {
    LOG_FUNC
    auto& reg = ge->get_reg();
    auto tv = ge->get_tv();
    ge->SetDrawTarget(ge->layer_wireframe.get());

    for(auto &&[ent, pos, size, wireframe] : reg.group<Pos, Size, Rendering::Wireframe>().each())
    {
        switch (wireframe.type) {
            case Rendering::Wireframe::TYPE::CIRCLE: {
                tv->DrawCircle(pos, size.h, wireframe.color);
            } break;
            case Rendering::    Wireframe::TYPE::CIRCLE_FILL: {
                tv->FillCircle(pos, size.h, wireframe.color);
            } break;
            case Rendering::Wireframe::TYPE::SQUARE: {
                // HACK for debugging a*
                tv->DrawRectDecal(pos+4, size-4, wireframe.color);
                // tv->DrawRectDecal(pos, size, wireframe.color);
            } break;
            case Rendering::Wireframe::TYPE::SQUARE_FILL: {
                tv->FillRectDecal(pos, size, wireframe.color);
            } break;
            case Rendering::Wireframe::TYPE::TRIANGLE: {
                auto side_opposite = (int) (size.h/sqrt(3));
                olc::vi2d pos2 = {static_cast<int>(pos.x+size.h - side_opposite),
                                  static_cast<int>(pos.y+size.h)};
                olc::vi2d pos3 = {static_cast<int>(pos.x+size.h + side_opposite),
                                  static_cast<int>(pos.y+size.h)};
                tv->DrawTriangle(pos, pos2, pos3);
            } break;
            case Rendering::Wireframe::TYPE::TRIANGLE_FILL: {
                auto side_opposite = (int) (size.h/sqrt(3));
                // olc::vi2d pos1 = {pos.x, pos.y};
                olc::vi2d pos2 = {static_cast<int>(pos.x + side_opposite), static_cast<int>(pos.y+size.h)};
                olc::vi2d pos3 = {static_cast<int>(pos.x - side_opposite), static_cast<int>(pos.y+size.h)};
                tv->FillTriangle(pos, pos2, pos3);
            } break;
            default:
                Error("Entity does not have a valid type {}", (int)wireframe.type);

        };
    }
}


void BackgroundRenderer::execute(TacticalGame* ge) {
    LOG_FUNC

    //if(!ge->animation_tick()) return;
    auto& reg = ge->get_reg();
    auto tv = ge->get_tv();
    ge->SetDrawTarget(ge->layer_bg.get());

    // for (auto &&[ent, pos, size, mng] :
    //      reg.group<Pos, Size, Rendering::RenderingManager, _bg_far>().each()) {
    //     Rendering::Spritesheet sheet;
    //     if(!tryget_component(reg, mng.sprite_sheet, sheet)) {
    //         Error("Error when attempting to get spritesheet");
    //         continue;
    //     }
    //     auto d = ge->get_decal(mng.index_decal);
    //     // if(ge->animation_tick()) Debug("Sprite sheet info {} {}", sheet.decal_index, sheet.animations_amt);

    //     if(!d) {
    //         Error("NO DECAL FOR {}", Debugging::entity_name(reg, ent));
    //         throw std::runtime_error("nullptr");
    //     }
    //     //d->UpdateSprite();
    //     // if(ge->animation_tick()) Debug("rendering pos {} size {} for entity {}",
    //     //                                mng.pos_sprite_sheet, sheet.pixel_frame_size.as_vf2d(),
    //     //                                Debugging::entity_name(reg, ent));
    //     tv->DrawPartialDecal(pos, d,
    //                          mng.pos_sprite_sheet,
    //                          sheet.pixel_frame_size,
    //                          mng.sprite_scale);
    // }

    // for(auto &&[ent, pos, size, mng] : reg.group<Pos, Size,
    //         Rendering::RenderingManager, Rendering::Layer::_bg_near>().each())
    // {
    //     Rendering::Spritesheet sheet;
    //     if(!tryget_component(reg, mng.sprite_sheet, sheet)) {
    //         Error("Error when attempting to get spritesheet");
    //         continue;
    //     }
    //     auto d = ge->get_decal(mng.index_decal);
    //     // if(ge->animation_tick()) Debug("Sprite sheet info {} {}", sheet.decal_index, sheet.animations_amt);

    //     if(!d) {
    //         Error("NO DECAL FOR {}", Debugging::entity_name(reg, ent));
    //         throw std::runtime_error("nullptr");
    //     }
    //     //d->UpdateSprite();
    //     // if(ge->animation_tick()) Debug("rendering pos {} size {} for entity {}",
    //     //                                mng.pos_sprite_sheet, sheet.pixel_frame_size.as_vf2d(),
    //     //                                Debugging::entity_name(reg, ent));
    //     tv->DrawPartialDecal(pos, d,
    //                          mng.pos_sprite_sheet,
    //                          sheet.pixel_frame_size,
    //                          mng.sprite_scale);

    // }

    ge->DrawString(10,10,"HELLO FROM 1", olc::VERY_DARK_RED, 2);
}
void render_furthest_layer(TacticalGame* ge) {
    auto& reg = ge->get_reg();
    auto tv = ge->get_tv();

    // for(auto &&[ent, pos, size, decal] : reg.view<Pos, Size, Rendering::Spritesheet, Rendering::Layer::_furthest>().each())
    // {
    //     auto d = ge->get_decal(decal.index);
    //     d->UpdateSprite();
    //     auto v2 = olc::vf2d(pos.x, pos.y);
    //     tv->DrawPartialDecal(v2, d,
    //                          olc::vf2d(100,100),
    //                          olc::vf2d(10,10));
    // }

    ge->DrawString(14,14,"HELLO FROM 2", olc::DARK_RED, 2);
}
void render_middle_layer(TacticalGame* ge) {
    auto& reg = ge->get_reg();
    auto tv = ge->get_tv();

    // for(auto &&[ent, pos, size, decal] : reg.view<Pos, Size, Rendering::Spritesheet, Rendering::Layer::_middle>().each())
    // {
    //     auto d = ge->get_decal(decal.index);
    //     d->UpdateSprite();
    //     auto v2 = olc::vf2d(pos.x, pos.y);
    //     tv->DrawPartialDecal(v2, d,
    //                          olc::vf2d(100,100),
    //                          olc::vf2d(10,10));
    // }

    ge->DrawString(14,14,"HELLO FROM 2", olc::DARK_RED, 2);
}
void render_closest_layer(TacticalGame* ge) {
    auto& reg = ge->get_reg();
    auto tv = ge->get_tv();

    // for(auto &&[ent, pos, size, decal] : reg.group<Pos, Size,
    //         Rendering::Spritesheet, Rendering::Layer::_closest>().each())
    // {
    //     auto d = ge->get_decal(decal.index);
    //     d->UpdateSprite();
    //     auto v2 = olc::vf2d(pos.x, pos.y);
    //     tv->DrawPartialDecal(v2, d,
    //                          olc::vf2d(100,100),
    //                          olc::vf2d(10,10));
    // }

    ge->DrawString(14,14,"HELLO FROM 2", olc::DARK_RED, 2);
}

void MainRenderer::execute(TacticalGame* ge) {
    LOG_FUNC
    ge->SetDrawTarget(ge->layer_main.get());

    render_furthest_layer(ge);
    render_middle_layer(ge);
    render_closest_layer(ge);
}

void PostRenderer::execute(TacticalGame* ge) {
    LOG_FUNC

}


void GUIRenderer::execute(TacticalGame* ge) {
    LOG_FUNC
    auto& reg = ge->get_reg();
    auto tv = ge->get_tv();
    ge->SetDrawTarget(ge->layer_gui.get());

    // for(auto &&[ent, pos, size] : reg.view<Pos, Size, Rendering::GUI::_container>().each())
    // {
    //     tv->DrawCircle(pos.x, pos.y, size.h);
    // }
}
