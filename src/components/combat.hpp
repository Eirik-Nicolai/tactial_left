#pragma once
#include "components.hpp"

class GameRegistry;

namespace Component::Combat
{

Component(Ability)

};

Component(StatusEffects)
};

Component(CombatState)
};

Component(CombatTurn)
};


Component(Moving) //{
    std::vector<Pos> dest_sequence;
    int sequence_step;

    Pos directional_speed; // TODO change type to smth else
    int moving_speed;
    std::function<void(GameRegistry*)> on_reached_dest;
};


Component(Node)
void add_neighbour(entt::entity neighbour)
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
};

// absolute pos of tile, i.e. (0,1), (3,4) etc
// might just use pos for this
Component(CurrentlyHolding) // tile is currently parent of entity
    entt::entity value;
};

Tag(Node_Start)
Tag(Node_End)


namespace Interaction {
    
Tag(Playable)
};

};
