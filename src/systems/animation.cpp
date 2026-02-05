#include "animation.hpp"

#include "components/components.hpp"
#include "utils/ecs.hpp"
void GUIAnimation::execute(TacticalGame* ge) {
    LOG_FUNC

    if(!ge->animation_tick()) return;

    auto& reg = ge->get_reg();

    // for(auto [ent, pos, size, decal] : reg.view<Pos, Size, Rendering::Decal,
    //         Rendering::Layer::_first>().each())
    // {

    // }
}

void CharacterAnimation::execute(TacticalGame* ge) {
    LOG_FUNC
    if(!ge->animation_tick()) return;

    auto& reg = ge->get_reg();
    for(auto [ent, mng] : reg.view<Rendering::Animation::AnimManager>().each())
    {
        Debug("Elapsed frames {} {} {}",mng.frames_elapsed, mng.index_curren_frame,
              mng.curr_animation.frames[mng.index_curren_frame].frame_duration);
        mng.frames_elapsed++;

        if(mng.frames_elapsed
           < mng.curr_animation.frames[mng.index_curren_frame].frame_duration)
        {
            continue; // nothing else to do
        }

        auto source_pos = mng.curr_animation.frames[mng.index_curren_frame].frame_pos;
        Debug("Source pos is {} {}", source_pos.x, source_pos.y);
        if(mng.curr_animation.frame_animation_length-1 > mng.index_curren_frame)
        {
            Debug("Moving frame for animation : {}, indx: {}", mng.curr_animation.name, mng.index_curren_frame);
            mng.frames_elapsed=0;
            mng.index_curren_frame++;

            Rendering::Spritesheet sheet;
            if(!tryget_component(reg, mng.sprite_sheet, sheet)) {
                Error("Error when attempting to get spritesheet");
                throw std::runtime_error("No sprite sheet for entity");
            }

            // TODO THIS ISN"T BEING SET IDK WHY
            // FIGURE OUT
            Rendering::RenderingManager rendering_mng;
            if(!tryget_component(reg, ent, rendering_mng)) {
                Error("Error when attempting to get rendering_mng");
                throw std::runtime_error("No sprite rendering_mng for entity");
            }

            auto source_pos = mng.curr_animation.frames[mng.index_curren_frame].frame_pos;
            auto abs_sprite_pos = (source_pos.as_vf2d()*sheet.pixel_frame_size.as_vf2d());
            rendering_mng.pos_sprite_sheet = abs_sprite_pos;
            Debug("Animation pos {} for entity {}", rendering_mng.pos_sprite_sheet, Debugging::entity_name(reg, ent));
            continue;
        }

        Debug("Is looping {}", mng.curr_animation.is_looping);
        if(mng.curr_animation.is_looping) {
            Debug("Looping for {}", mng.name);
            mng.index_curren_frame = 0;
            mng.frames_elapsed=0;
            continue;
        }
    }
}

void BGAnimation::execute(TacticalGame* ge) {
    LOG_FUNC

    if(!ge->animation_tick()) return;
    auto& reg = ge->get_reg();

    // for(auto [ent, pos, size, decal] : reg.view<Pos, Size, Rendering::Decal, Rendering::Layer::_second>().each())
    // {

    // }
}
