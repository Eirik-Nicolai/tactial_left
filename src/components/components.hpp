#pragma once
#include "olc/olcPixelGameEngine.h"
#include "entt/entt.hpp"

struct Component {
  Component() = default;
  virtual std::string print() const = 0;
};


namespace Rendering {
  struct Texture {};

  struct Size{
    int h;
    int w;
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
  struct Id {
    unsigned value;
  };
  struct Print {
    std::string name;
  };
};

namespace World {
};
struct Pos {
  int x;
  int y;
};
struct SizeCirc {
  int r;
};
struct SizeRect {
  int w;
  int h;
};

struct Orbiting {
  entt::entity anchor;
  float dist;
  float speed;
  float angle;
};


namespace Tag {
struct Hoverable{};
struct Hovered{};
struct Selectable{};
struct Selected{};

struct Star{};
struct Planet{};
struct Moon{};



};
