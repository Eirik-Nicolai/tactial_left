#ifndef ANIMATION_H_
#define ANIMATION_H_

#include "components.hpp"

#include <string>

namespace Component::Animation
{
Component(AnimationFrame)
    Pos frame_pos;      // based on relative frame of the spritesheet, not pixel pos
    int frame_duration; // in frames
};
// add state ?
Component(SpriteSheetAnimation)
    std::string animation_name;
    std::array<AnimationFrame, 40> frames;
    int frame_animation_length;
    bool is_looping;
    bool is_flipped;
};

Component(AnimationList)
    unsigned animations_amt = 0;
    std::array<Animation::SpriteSheetAnimation, 40> animations;
};

/// TODO rename these
Component(AnimManager)
    int frame_duration;
    SpriteSheetAnimation curr_animation;

    int frames_elapsed;

    size_t index_curren_frame;
    size_t index_curren_animation;

    entt::entity sprite_sheet;
};

}; // namespace Animation

#endif // ANIMATION_H_
