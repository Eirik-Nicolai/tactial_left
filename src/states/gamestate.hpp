#pragma once
#include "engine/input_handler.hpp"
#include "engine/event.hpp"
#include "engine/game_input_event.hpp"
#include "utils/debugging.hpp"

class InputHandler;
class TacticalGame;

class GameState
{
    GET_BASE(GameState)
  public:
    GameState(std::shared_ptr<GameRegistry> reg) : m_registry(reg) {};
    virtual ~GameState(){};
    // GameState(const GameState   & other) = delete;
    
  public:
    virtual std::string get_name() const { return "GameState"; }

    // ------ SINGLE RUN FUNCTIONS ------
    virtual void pause() {}  // pause logic, if needed
    virtual void resume() {} // resume logic, if paused

    // ------ LOOPING FUNCTIONS ------
    virtual void handle_input(Engine::Event &) {} // handle all user input, if any
    virtual void update() {} // update and handle logic of state
    virtual void draw(TacticalGame *) {}   // draw state to screen
    // virtual void on_event(TacticalGame* ge, Event& event) {}        // events incoming
    
  protected:
    // GameState() {}
    std::unique_ptr<InputHandler> handler;
    std::shared_ptr<GameRegistry> m_registry;
};
