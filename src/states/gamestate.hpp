#pragma once
#include "game.hpp"
class TacticalGame;

class GameState {
    public:
        std::string get_name() { return "GameState"; }

        virtual void init(TacticalGame* ge) {}
        virtual void cleanup(TacticalGame* ge) {}

        virtual void enter(TacticalGame* ge) {}
        virtual void exit(TacticalGame* ge) {}

        virtual void pause(TacticalGame* ge) {}
        virtual void resume(TacticalGame* ge) {}

        virtual void handle_input(TacticalGame* ge) {}

        virtual void draw(TacticalGame* ge) {}
        virtual void update(TacticalGame* ge) {} // ?
};
