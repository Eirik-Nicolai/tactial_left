#include "animation.hpp"

#include "components/components.hpp"
void GUIAnimation::execute(TacticalGame* ge) {
    LOG_FUNC

    auto& reg = ge->get_reg();

    for(auto [ent, pos, size, decal] : reg.view<Pos, Size, Rendering::Decal,
            Rendering::Layer::_first>().each())
    {

    }
}

void CharacterAnimation::execute(TacticalGame* ge) {
    LOG_FUNC

    auto time_since_last_frame = ge->GetElapsedTime();
    auto& reg = ge->get_reg();
    for(auto [ent, anim] : reg.view<Rendering::Animation::Anim>().each())
    {
        anim.current_duration_elapsed+=time_since_last_frame;

        if(anim.current_duration_elapsed < anim.frames[anim.curren_frame].duration)
        {
            continue; // nothing else to do
        }
        if(anim.amt_frames > anim.curren_frame
           && anim.curren_frame < anim.frames.size()) //prevent out of bounds
        {
            Debug("Moving frame for animation {}", anim.name);
            anim.current_duration_elapsed=0;
            anim.curren_frame++; // nothing else to do
            continue;
        }
        if(anim.looping) {
            Debug("Looping for {}", anim.name);
            anim.curren_frame = 0;
            continue;
        }

        // TODO we can set the pos/size to a rendering component here
        // so we can easily extract it in the rendering system hot path
    }
}

void BGAnimation::execute(TacticalGame* ge) {
    LOG_FUNC

    auto& reg = ge->get_reg();

    for(auto [ent, pos, size, decal] : reg.view<Pos, Size, Rendering::Decal, Rendering::Layer::_second>().each())
    {

    }
}
