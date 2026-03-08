#include "game.hpp"

#include "logger.hpp"

#include "asset_manager.hpp"
#include "components/components.hpp"
#include "components/rendering.hpp"
#include "components/gui.hpp"

using namespace Component;
void PreRenderer(TacticalGame *ge)
{
    GET_NAME_FUNC(PreRenderer);
    // ge->Clear(olc::BLACK);
    // any other init rendering step, might not be needed
}

void BackgroundRenderer(TacticalGame *ge)
{
    GET_NAME_FUNC(BackgroundRenderer);
    // if(!ge->animation_tick()) return;
    auto reg = ge->registry();
    auto tv = ge->get_tv();
    ge->SetDrawTarget(ge->layer_bg.get());
    ge->DrawString(10, 10, "HELLO FROM 1", olc::VERY_DARK_RED, 2);
}
void render_furthest_layer(TacticalGame *ge)
{
    auto reg = ge->registry();
    auto tv = ge->get_tv();

    // for(auto &&[ent, pos, size, decal] : reg->get().view<Pos, Size, Rendering::Spritesheet,
    // Rendering::Layer::_furthest>().each())
    // {
    //     auto d = ge->get_decal(decal.index);
    //     d->UpdateSprite();
    //     auto v2 = olc::vf2d(pos.x, pos.y);
    //     tv->DrawPartialDecal(v2, d,
    //                          olc::vf2d(100,100),
    //                          olc::vf2d(10,10));
    // }

    ge->DrawString(14, 14, "HELLO FROM 2", olc::DARK_RED, 2);
}
void render_middle_layer(TacticalGame *ge)
{
    auto reg = ge->registry();
    auto tv = ge->get_tv();

    // for(auto &&[ent, pos, size, decal] : reg->get().view<Pos, Size, Rendering::Spritesheet,
    // Rendering::Layer::_middle>().each())
    // {
    //     auto d = ge->get_decal(decal.index);
    //     d->UpdateSprite();
    //     auto v2 = olc::vf2d(pos.x, pos.y);
    //     tv->DrawPartialDecal(v2, d,
    //                          olc::vf2d(100,100),
    //                          olc::vf2d(10,10));
    // }

    ge->DrawString(14, 14, "HELLO FROM 2", olc::DARK_RED, 2);
}

void PostRenderer(TacticalGame *ge) {
}

void TacticalGame::System_Render(float deltatime)
{
    using namespace Component;
    
    ///////////////////////////////////////////////////////////////////////////////////
    // RENDER CLOSEST LAYER
    std::function<std::string(void)> get_name = []() { return "render_closest_layer"; };
    
    for (auto &&[ent, pos, size, sheet, mng] :
         m_registry->get()
             .view<Pos, Size, Rendering::Spritesheet, Rendering::RenderingManager>()
             .each()) {
        if (animation_tick()) {
            Trace("Entity(" << m_registry->entity_name(ent) << ") "
                            << "pos_sprite_sheet:" << mng.pos_sprite_sheet);
        }
        auto decal = AssetManager::instance().get_spritesheet(sheet.decal_index);
        // d->UpdateSprite(); idk if this is needed
        tvp->DrawPartialDecal(pos, decal.get(), mng.pos_sprite_sheet, sheet.pixel_frame_size,
                             {3.f, 3.f});
    }
    DrawString(14, 14, "HELLO FROM 2", olc::DARK_RED, 2);

    
    ///////////////////////////////////////////////////////////////////////////////////
    // GUI RENDERER
    get_name = []() { return "render_closest_layer"; };
    SetDrawTarget(layer_gui.get());

    for (auto &&[ent, pos, size] :
         m_registry->get().view<GUI::Pos, GUI::Size, GUI::_is_gui>().each()) {
        std::shared_ptr<olc::Decal> d;
        if (auto ptr = m_registry->get_component<Rendering::Texture>(ent); ptr != nullptr) {
            d = AssetManager::instance().get_texture(ptr->decal_index);
        }
        if (auto ptr = m_registry->get_component<Rendering::TextureList>(ent); ptr != nullptr) {
            auto index = ptr->textures[ptr->curr_texture];
            d = AssetManager::instance().get_texture(index);
        }
        FillRect(pos.x, pos.y, 40, 40, olc::RED);
        // Info("Decal h:" << d->height << " id:" << d->id);
        DrawDecal(pos, d.get());
    }


    ///////////////////////////////////////////////////////////////////////////////////
    // WIREFRAME RENDERING -- debugging
    get_name = []() { return "render_wireframe"; };
    SetDrawTarget(layer_wireframe.get());

    for (auto &&[ent, pos, size, wireframe] :
         m_registry->get().view<Pos, Size, Rendering::Wireframe>().each()) {
        switch (wireframe.type) {
        case Rendering::Wireframe::TYPE::CIRCLE: {
            tvp->DrawCircle(pos, size.h, wireframe.color);
        } break;
        case Rendering::Wireframe::TYPE::CIRCLE_FILL: {
            tvp->FillCircle(pos, size.h, wireframe.color);
        } break;
        case Rendering::Wireframe::TYPE::SQUARE: {
            // HACK for debugging a*
            tvp->DrawRectDecal(pos + 4, size - 4, wireframe.color);
            // tvp->DrawRectDecal(pos, size, wireframe.color);
        } break;
        case Rendering::Wireframe::TYPE::SQUARE_FILL: {
            tvp->FillRectDecal(pos, size, wireframe.color);
        } break;
        case Rendering::Wireframe::TYPE::TRIANGLE: {
            auto side_opposite = (int)(size.h / sqrt(3));
            olc::vi2d pos2 = {static_cast<int>(pos.x + size.h - side_opposite),
                              static_cast<int>(pos.y + size.h)};
            olc::vi2d pos3 = {static_cast<int>(pos.x + size.h + side_opposite),
                              static_cast<int>(pos.y + size.h)};
            tvp->DrawTriangle(pos, pos2, pos3);
        } break;
        case Rendering::Wireframe::TYPE::TRIANGLE_FILL: {
            auto side_opposite = (int)(size.h / sqrt(3));
            // olc::vi2d pos1 = {pos.x, pos.y};
            olc::vi2d pos2 = {static_cast<int>(pos.x + side_opposite),
                              static_cast<int>(pos.y + size.h)};
            olc::vi2d pos3 = {static_cast<int>(pos.x - side_opposite),
                              static_cast<int>(pos.y + size.h)};
            tvp->FillTriangle(pos, pos2, pos3);
        } break;
        default:
            Error("Entity does not have a valid type " << (int)wireframe.type);
        };
    }
}
