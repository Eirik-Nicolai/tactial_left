#pragma once

#include "entt/entt.hpp"

/// TODO maybe move to macros
template <typename component>
bool has(entt::registry& reg, entt::entity &ent)
{
  return reg.all_of<component>(ent);
}

template <typename component>
bool tryget_component(entt::registry& reg, entt::entity &ent, component &comp)
{
  if(auto getcomp = reg.try_get<component>(ent);reg.try_get<component>(ent) != nullptr)
  {
    comp = (*getcomp);
    return true;
  }
  return false;
}

// template <typename component>
// bool tryget_component(const entt::registry& reg, const entt::entity &ent, component &comp)
// {
//   if(auto getcomp = reg.try_get<component>(ent);reg.try_get<component>(ent) != nullptr)
//   {
//     comp = (*getcomp);
//     return true;
//   }
//   return false;
// }

//TODO check reference
template <typename component>
component get(entt::registry& reg, entt::entity e)
{
  return reg.get<component>(e);
}

template <typename component>
const component cget(entt::registry& reg, entt::entity e)
{
  return reg.get<component>(e);
}

template <typename component>
component add(entt::registry& reg, entt::entity e)
{
  return reg.emplace<component>(e);
}

template <typename component, typename ...Args>
component add(entt::registry& reg, entt::entity e, Args && ...args)
{
  return reg.emplace<component>(e, std::forward<Args>(args) ...);
}





#include "components/components.hpp"
namespace Debugging {
  inline std::string entity_name(entt::registry& reg, entt::entity& ent) {
    Debugging::Debug d;
    auto res = tryget_component(reg, ent, d);
    if(!res) return "[NAMELESS ENTITY]";
    return d.name;
  }
  inline uint32_t entity_id(entt::registry& reg, entt::entity& ent) {
    Debugging::Debug d;
    auto res = tryget_component(reg, ent, d);
    if(!res) return 0;
    return d.id;
  }
};
