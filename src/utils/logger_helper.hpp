#pragma once

#include "components/components.hpp"
#include "components/animation.hpp"
#include "components/rendering.hpp"
#include "components/debugging.hpp"
#include "utils/debugging.hpp"
#include <entt/entt.hpp>
#include <ios>

inline std::ostream &operator<<(std::ostream &os, entt::entity &e)
{
    // TODO 
    return os;
}

inline std::ostream &operator<<(std::ostream &os, Size &s)
{
    os << "Size(h:"<<s.h<<", w:"<<s.w<<")";
    return os;
}

inline std::ostream &operator<<(std::ostream &os, Pos &p)
{
    os << "Pos(x:"<<p.x<<", y:"<<p.y<<")";
    return os;
}
