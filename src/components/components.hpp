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

  Size operator+(Size const& rhs) { return Size{w+rhs.w,w+rhs.w}; };
  template <typename RHS>
  Size operator+(RHS const& rhs) { return Size{w+rhs,w+rhs}; };

  Size operator-(Size const& rhs) { return Size{w-rhs.w,w-rhs.w}; };
  template <typename RHS>
  Size operator-(RHS const& rhs) { return Size{w-rhs,w-rhs}; };

  // Size operator- { return olc::vf2d(h,w); };
  // Size operator* { return olc::vf2d(h,w); };
  // Size operator/ { return olc::vf2d(h,w); };

  operator olc::vf2d() const { return olc::vf2d(h,w); };
  operator olc::vi2d() const { return olc::vi2d(h,w); };
};
struct Pos{
  float x;
  float y;

  Pos operator+(Pos const& rhs) { return Pos{x+rhs.x,y+rhs.y}; };
  template <typename RHS>
  Pos operator+(RHS const& rhs) { return Pos{x+rhs,y+rhs}; };

  Pos operator-(Pos const& rhs) { return Pos{x-rhs.x,y-rhs.y}; };
  template <typename RHS>
  Pos operator-(RHS const& rhs) { return Pos{x-rhs,y-rhs}; };

  operator olc::vf2d() const { return olc::vf2d(x,y); };
  operator olc::vi2d() const { return olc::vi2d(x,y); };
};

// size and pos in screenspace not game space
// mostly for gui ?
namespace Screen{
  struct Size{
    float h;
    float w;
  operator olc::vf2d() const { return olc::vf2d(h,w); };
  operator olc::vi2d() const { return olc::vi2d(h,w); };
  };
  struct Pos{
    float x;
    float y;
  operator olc::vf2d() const { return olc::vf2d(x,y); };
  operator olc::vi2d() const { return olc::vi2d(x,y); };
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

  namespace Terrain {

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

namespace FSM {
  struct StateManager {

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
namespace Gameplay {
  namespace Interaction {
    struct _hoverable{};
    struct _hovered{};
    struct _selectable{};
    struct _selected{};
  };
};
struct _star{};
struct _planet{};
struct _moon{};


namespace Combat{
  namespace Terrain {
    struct _path{};
    struct _obstruction{};
  };
};
