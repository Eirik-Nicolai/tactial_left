#pragma once

#include "components/components.hpp"
#include "engine/event.hpp"
#include <entt/entt.hpp>
#include <ios>
#include "engine/mouse_event.hpp"
inline std::ostream &operator<<(std::ostream &os, Engine::Event &e)
{
    os << "Event(" << e.get_name() << ")";
    return os;
}
inline std::ostream &operator<<(std::ostream &os, entt::entity e)
{
    os << "Entity(" << (uint32_t)e << ")";
    return os;
}
inline std::ostream &operator<<(std::ostream &os, Component::Size &s)
{
    os << "Size(h:"<<s.h<<", w:"<<s.w<<")";
    return os;
}
inline std::ostream &operator<<(std::ostream &os, Component::Pos &p)
{
    os << "Pos(x:"<<p.x<<", y:"<<p.y<<")";
    return os;
}

inline std::ostream &operator<<(std::ostream &os, Engine::MouseButtonPressedEvent &e)
{
    os << e.to_string();
    return os;
}

inline std::ostream &operator<<(std::ostream &os, Engine::MouseButtonReleasedEvent &e)
{
    os << e.to_string();
    return os;
}
