#pragma once
#include "engine/input_handler.hpp"

class InputHandler;
class TacticalGame;

class GameState {
  public:
        GameState(GameState const&) = delete;
        GameState operator=(GameState const&) = delete;

  public:
        virtual std::string get_name() const { return "GameState"; }

        // ------ SINGLE RUN FUNCTIONS ------
        virtual void init(TacticalGame* ge) {}          // initialize state start of game
        virtual void cleanup(TacticalGame* ge) {}       // clean up state at end of game

        virtual void enter(TacticalGame* ge) {}         // set up to enter state during game
        virtual void exit(TacticalGame* ge) {}          // set up to switch state during game

        virtual void pause(TacticalGame* ge) {}         // pause logic, if needed
        virtual void resume(TacticalGame* ge) {}        // resume logic, if paused

        // ------ LOOPING FUNCTIONS ------
        virtual void handle_input(TacticalGame* ge) {}  // handle all user input, if any
        virtual void update(TacticalGame* ge) {}        // update and handle logic of state
        virtual void draw(TacticalGame* ge) {}          // draw state to screen

    protected:
        GameState() {}
        InputHandler* handler;
};
