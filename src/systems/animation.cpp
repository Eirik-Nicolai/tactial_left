#include "animation.hpp"
#include "utils/ecs.hpp"
#include "components/animation.hpp"
#include "components/rendering.hpp"

void GUIAnimation::execute(TacticalGame *ge)
{

    if (!ge->animation_tick())
        return;

    auto reg = ge->registry();

    // for(auto [ent, pos, size, decal] : reg.view<Pos, Size, Rendering::Decal,
    //         Rendering::Layer::_first>().each())
    // {

    // }
}

void CharacterAnimation::execute(TacticalGame *ge)
{
    if (!ge->animation_tick())
        return;
    Trace("Animation manager");
    auto reg = ge->registry();
    for (auto &&[ent, mng] : reg->get().view<Animation::AnimManager>().each()) {
        Trace("Elapsed frames "
              << mng.frames_elapsed << " / "
              << mng.curr_animation.frames[mng.index_curren_frame].frame_duration);

        if (mng.frames_elapsed <
            mng.curr_animation.frames[mng.index_curren_frame].frame_duration) {
            mng.frames_elapsed++;
            continue; // nothing else to do
        }

        auto source_pos = mng.curr_animation.frames[mng.index_curren_frame].frame_pos;
        Trace("Source pos is " << source_pos.x << " " << source_pos.y);
        if (mng.curr_animation.frame_animation_length - 1 > mng.index_curren_frame) {
            Trace("Moving frame for animation :" << mng.curr_animation.name
                                                 << " indx:" << mng.index_curren_frame);
            mng.frames_elapsed = 0;
            mng.index_curren_frame++;

            if (auto sheet = reg->get_component<Rendering::Spritesheet>(ent);
                sheet != nullptr) {
                
                auto rmng = reg->get_component<Rendering::RenderingManager>(ent);

                auto source_pos =
                    mng.curr_animation.frames[mng.index_curren_frame].frame_pos;
                auto abs_sprite_pos =
                    ((olc::vf2d)source_pos * (olc::vf2d)sheet->pixel_frame_size);
                rmng->pos_sprite_sheet = abs_sprite_pos;
                rmng->pos_sprite_sheet.x = abs_sprite_pos.x;
                rmng->pos_sprite_sheet.y = abs_sprite_pos.y;

                Trace("Animation pos " << rmng->pos_sprite_sheet << " for entity "
                                       << reg->entity_name(ent));
                continue;
            }
            Error("Error when attempting to get spritesheet in animation for entity "
                  << reg->entity_name(ent));
            throw std::runtime_error("No sprite sheet for entity");

            // // nmake helper function for this ? unsure why tryget_component does not
            // // work for this when its non const
            // for (auto [ent, rmng] :
            // reg-get().group<Rendering::RenderingManager>().each()) {

            //     // TODO THIS ISN"T BEING SET IDK WHY
            //     // FIGURE OUT
            //     auto source_pos =
            //         mng.curr_animation.frames[mng.index_curren_frame].frame_pos;
            //     auto abs_sprite_pos =
            //         ((olc::vf2d)source_pos * (olc::vf2d)sheet.pixel_frame_size);
            //     rmng.pos_sprite_sheet = abs_sprite_pos;
            //     Trace("Animation pos " << rmng.pos_sprite_sheet << " for entity "
            //                            << Debugging::entity_name(reg, ent));
            // }
        }

        if (mng.curr_animation.is_looping) {
            Trace("Looping for " << mng.name);
            mng.index_curren_frame = 0;
            mng.frames_elapsed = 0;
            continue;
        }
    }
}

void BGAnimation::execute(TacticalGame *ge)
{
    if (!ge->animation_tick())
        return;
    auto reg = ge->registry();

    // for(auto [ent, pos, size, decal] : reg.view<Pos, Size, Rendering::Decal,
    // Rendering::Layer::_second>().each())
    // {

    // }
}
