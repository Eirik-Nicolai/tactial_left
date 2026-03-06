#pragma once
#include <memory>

// struct state_change {
//     GameState& geir_gustav;
// };

// struct state_change_func {
//     std::function<GameState&(void)> geir_gustav;
// };

class GameState;
struct do_state_change_playaction {
    //TacticalGame game;
    // std::shared_ptr<GameRegistry> reg;
    //int i;
};

struct do_state_change_current {
    int index;
    std::unique_ptr<GameState> new_state;
};
struct do_state_change_add_overlay {
    std::unique_ptr<GameState> new_state;
};
