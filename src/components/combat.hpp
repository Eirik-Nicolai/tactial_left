#pragma once
#include "components.hpp"
// clang-format off
class GameRegistry;

namespace Component::Combat
{

namespace World
{

Component(TileInfo)
    Size s;
};

Component(CombatTurn)
};

}; //world

Component(BaseStats)
    int health;
    int mana;
    int defense;    // diff types ?
    int move_speed; // in tile amt
    int speed;      // how often is my turn
};

Tag(Ability)
Tag(Buff)
Tag(Debuff)
Tag(Not_removable)

// mostly for debugging, this should be centralized somewhere 
Component(Name)
};
Component(Icon)
};

// ability applis a direct modifier on a stat
// I.E. decrease health, increase defense, etc
Component(ModifyStat)
    // stat modifiers are affected by type (TODO own component or enum)
    float value;
    int type;
};


// a status effect entity can have one or more of these
namespace StatusEffect {
Component(Duration)
    int remaining_turns;
    int duration_turns;
};

// Modify stat
};

// an ability entity can have one or more of these
namespace Ability
{
Component(Cost)
    float value;
    // health, mana, other
};
    
Component(ApplyStatusEffects)
    std::vector<entt::entity> effects;
};

// we target tiles, not other player entities
Component(TargetingSingular)
    int amt_targets;
};
Component(TargetingArea_Circular)
    int tile_radius;
};

Component(TargetingArea_Rectangular)
    int tile_width;
    int tile_height;
};

Component(CanCrit)
    float luck;
    float crit_modifier;
};

Component(Does)
    std::array<entt::entity, 20> effects; 
};
// RENDERING abilities have an animation and asset component
// Modify stat
};


Component(Moving) //{
    std::vector<Pos> dest_sequence;
    int sequence_step;

    Pos directional_speed; // TODO change type to smth else
    int moving_speed;
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
// clang-format on
