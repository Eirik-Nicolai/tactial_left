#pragma once
#include "olc/olcPixelGameEngine.h"
#include "entt/entt.hpp"

namespace Rendering {
  struct Texture {};

  struct Wireframe{
    olc::Pixel color;
    bool is_circular;
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
  olc::vf2d coordinates;
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
