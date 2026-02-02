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
struct Pos {
  int x;
  int y;
};
struct Size{
  int h;
  int w;
};

// size and pos in screenspace not game space
// mostly for gui ?
namespace Screen{
  struct Size{
    float h;
    float w;
  };
  struct Pos{
    float x;
    float y;
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
  struct Decal {
    olc::Decal* value;
    Screen::Size size;
    Screen::Pos pos; // if needed
  };
  struct Background {
    olc::Decal value;
  };

  namespace Animation {
    struct Anim {
      struct AnimationFrame {
        Pos pos;
        Size size;
        float duration;
      };
      std::string name;
      std::array<AnimationFrame, 40> frames;
      float current_duration_elapsed;
      bool looping;
      size_t amt_frames;
      size_t curren_frame;
    };

  };

  namespace GUI {



  };
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
