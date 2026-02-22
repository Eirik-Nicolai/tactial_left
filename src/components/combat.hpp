#pragma once
#include "components.hpp"
namespace Component::Combat
{

Component(Ability)

};

Component(StatusEffects)
}
;

Component(CombatState)
}
;

Component(CombatTurn)
}
;

Component(Node) void add_neighbour(entt::entity neighbour)
{
    if (neighbour_count < neighbours.size())
        neighbours[neighbour_count++] = neighbour;
}
bool is_obstacle;
bool is_visited;
float global_goal;
float local_goal;
size_t neighbour_count;
std::array<entt::entity, 8> neighbours;
entt::entity parent;

unsigned weight;
}
;

Tag(Node_Start) Tag(Node_End)
}
