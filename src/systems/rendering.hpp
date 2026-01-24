#pragma once
#include "game.hpp"
#include "entt/entt.hpp"

// TODO move systems to subdir of states probably
// except global ones if there are any ?

namespace State {
namespace Star {

void render_stars(entt::registry &reg, TacticalGame* ge);
void render_gui(entt::registry &reg, TacticalGame* ge);
void apply_shaders(entt::registry &reg, TacticalGame* ge);

};
};
