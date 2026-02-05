#pragma once
#include "olc/olcPixelGameEngine.h"
#include "entt/entt.hpp"

#include <string>

// ------------------------------------------------
// |    COMPONENTS FOR ECS SYSTEM MAIN FILE
// |    Naming:
// |    Tags: _name
// |    Logic: Name
// ------------------------------------------------

struct Component {
  Component() = default;
  virtual std::string print() const = 0;
};
struct Size{
  float h;
  float w;
  olc::vf2d as_vf2d() { return olc::vf2d(h,w); }
  olc::vf2d as_vi2d() { return olc::vi2d(h,w); }
};
struct Pos{
  int x;
  int y;
  olc::vf2d as_vf2d() { return olc::vf2d(x,y); }
  olc::vf2d as_vi2d() { return olc::vi2d(x,y); }
};

// size and pos in screenspace not game space
// mostly for gui ?
namespace Screen{
  struct Size{
    float h;
    float w;
    olc::vf2d as_vf2d() { return olc::vf2d(h,w); }
    olc::vf2d as_vi2d() { return olc::vi2d(h,w); }
  };
  struct Pos{
    float x;
    float y;
    olc::vf2d as_vf2d() { return olc::vf2d(x,y); }
    olc::vf2d as_vi2d() { return olc::vi2d(x,y); }
  };
}

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
  // playing states (name pending, parent states ?) keep track of sprites which are
  // loaded on enter and passed to decals
  // which are sent to renderer system to render in correct layer
  struct Background {
    olc::Decal value;
  };

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

namespace Debugging {
  struct Debug{
    Debug() {};
    Debug(std::string s) : name(s)
    { id=std::hash<std::string>{}(s); }

    std::string name;
    uint32_t id;
  };
};

namespace World {
};
struct Orbiting {
  entt::entity anchor;
  float dist;
  float speed;
  float angle;
};
// TODO move to game logic namespace and file
struct _hoverable{};
struct _hovered{};
struct _selectable{};
struct _selected{};

struct _star{};
struct _planet{};
struct _moon{};
