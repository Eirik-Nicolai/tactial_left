#pragma once
#include "engine/input_handler.hpp"
#include "engine/event.hpp"
#include "engine/event.hpp"
#include "utils/debugging.hpp"

class InputHandler;
class TacticalGame;

class GameState {
    GET_BASE(GameState)
  public:
        GameState() {};
        virtual ~GameState() {};
  public:
        virtual std::string get_name() const { return "GameState"; }

        // ------ SINGLE RUN FUNCTIONS ------
        virtual void enter(TacticalGame* ge) {}         // set up to enter state during game
        virtual void exit(TacticalGame* ge) {}          // set up to switch state during game

        virtual void pause(TacticalGame* ge) {}         // pause logic, if needed
        virtual void resume(TacticalGame* ge) {}        // resume logic, if paused

        // ------ LOOPING FUNCTIONS ------
        virtual void handle_input(TacticalGame* ge, Event &) {}  // handle all user input, if any
        virtual void update(TacticalGame* ge) {}        // update and handle logic of state
        virtual void draw(TacticalGame* ge) {}          // draw state to screen
        // virtual void on_event(TacticalGame* ge, Event& event) {}        // events incoming

    protected:
        // GameState() {}
        std::unique_ptr<InputHandler> handler;
};
