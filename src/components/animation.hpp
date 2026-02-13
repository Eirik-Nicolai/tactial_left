#ifndef ANIMATION_H_
#define ANIMATION_H_

#include "components.hpp"

#include <string>

namespace Animation
{
struct AnimationFrame {
    Pos frame_pos;      // based on relative frame of the spritesheet, not pixel pos
    int frame_duration; // in frames
};
// add state ?
struct SpriteSheetAnimation {
    std::string name;
    std::array<AnimationFrame, 40> frames;
    int frame_animation_length;
    bool is_looping;
    bool is_flipped;
};

struct AnimationList {
    unsigned animations_amt;
    std::array<Animation::SpriteSheetAnimation, 40> animations;
};

/// TODO rename these
struct AnimManager {
    std::string name;
    int frame_duration;
    SpriteSheetAnimation curr_animation;

    int frames_elapsed;

    size_t index_curren_frame;
    size_t index_curren_animation;

    entt::entity sprite_sheet;
};

}; // namespace Animation

#endif // ANIMATION_H_
