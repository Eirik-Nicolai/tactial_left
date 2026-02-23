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

#define Component(_1)                                                                    \
    struct _1 {                                                                          \
        static std::string title() { return "Component_" #_1; }

#define Tag(_1)                                                                          \
    struct _##_1 {                                                                       \
        static std::string title() { return "Tag" #_1; }                                 \
    };

namespace Component
{

Component(Size)
int w;
int h;

Size operator+(Size const &rhs) { return Size{.w = w + rhs.w, .h = h + rhs.h}; };
template <typename RHS>
Size operator+(RHS const &rhs)
{
    return Size{.w = w + rhs, .h = h + rhs};
};
Size operator-(Size const &rhs) { return Size{.w = w - rhs.w, .h = h - rhs.h}; };
template <typename RHS>
Size operator-(RHS const &rhs)
{
    return Size{.w = w - rhs, .h = h - rhs};
};

// Size operator- { return olc::vf2d(h,w); };
// Size operator* { return olc::vf2d(h,w); };
// Size operator/ { return olc::vf2d(h,w); };

operator olc::vf2d() const { return olc::vf2d(h, w); };
operator olc::vi2d() const { return olc::vi2d(h, w); };
};

Component(Pos)
int x;
int y;

Pos operator+(Pos const &rhs) { return Pos{.x = x + rhs.x, .y = y + rhs.y}; };
template <typename RHS>
Pos operator+(RHS const &rhs)
{
    return Pos{.x = x + rhs, .y = y + rhs};
};
Pos operator-(Pos const &rhs) { return Pos{.x = x - rhs.x, .y = y - rhs.y}; };
template <typename RHS>
Pos operator-(RHS const &rhs)
{
    return Pos{.x = x - rhs, .y = y - rhs};
};

// Pos operator+(Pos const& rhs) { x+=rhs.x;y+=rhs.y; return *this; };
// template <typename RHS>
// Pos operator+(RHS const& rhs) { x+=rhs;y+=rhs; return *this; };

// Pos operator-(Pos const& rhs) { x-=rhs.x;y-=rhs.y; return *this; };
// template <typename RHS>
// Pos operator-(RHS const& rhs) { x-=rhs;y-=rhs; return *this; };

operator olc::vf2d() const { return olc::vf2d(x, y); };
operator olc::vi2d() const { return olc::vi2d(x, y); };
};

Component(Renderable)
Pos pos;
Size size;
};

namespace World
{
};
struct Orbiting {
    entt::entity anchor;
    float dist;
    float speed;
    float angle;
};
struct _star {
};
struct _planet {
};
struct _moon {
};

namespace Combat
{
namespace Terrain
{
Tag(path) struct _obstruction {
};
}; // namespace Terrain
}; // namespace Combat
}
