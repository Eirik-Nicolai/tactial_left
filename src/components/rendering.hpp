#ifndef RENDERING_H_
#define RENDERING_H_

#include "components.hpp"

namespace Animation {
struct AnimationFrame {
  Pos frame_pos; // based on relative frame of the spritesheet, not pixel pos
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
};


namespace Rendering {
namespace Layer {
  struct _pre{};
  struct _first{};
  struct _second{};
  struct _third{};
  struct _post{};
  struct _gui{};
  struct _wireframe{};
};

namespace Terrain {

};

struct Spritesheet {
  unsigned decal_index;
  Size pixel_frame_size;
  unsigned animations_amt;
  std::array<Animation::SpriteSheetAnimation, 40> animations;
};

struct RenderingManager{
  entt::entity sprite_sheet;
  olc::vf2d pos_sprite_sheet;
  olc::vf2d sprite_scale;
  unsigned index_decal;
};

namespace GUI {};
struct Wireframe{
  enum class TYPE{
    CIRCLE,
    SQUARE,
    TRIANGLE,
    CIRCLE_FILL,
    SQUARE_FILL,
    TRIANGLE_FILL,
  } type;
  olc::Pixel color;
};
};


#endif // RENDERING_H_
