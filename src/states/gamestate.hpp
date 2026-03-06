#pragma once
#include "engine/input_handler.hpp"
#include "engine/event.hpp"
#include "engine/game_input_event.hpp"
#include "utils/debugging.hpp"
#include "registry/game_event.hpp"

class InputHandler;
class TacticalGame;

class GameState
{
    GET_BASE(GameState)
  public:
    GameState(TacticalGame* ge, std::shared_ptr<GameRegistry> reg, int index=-1) : m_registry(reg), m_game(ge), m_index(index) {};
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
    virtual void draw() {}   // draw state to screen

    void connect_signals(int index,
                         std::function<void(do_state_change_add_overlay)> signal_add,
                         std::function<void(do_state_change_current)> signal_change)
    {
        m_index = index;
        m_signal_state_add = signal_add;
        m_signal_state_change = signal_change;
    }

protected:
    // GameState() {}
    std::unique_ptr<InputHandler> handler;
    std::shared_ptr<GameRegistry> m_registry;
    TacticalGame* m_game;

    // TODO rename these
    template <typename NewState>
    void m_change_state_to()
    {
        try {
            m_signal_state_change(do_state_change_current{
                .index = m_index,
                .new_state = std::move(std::make_unique<NewState>(m_game, m_registry))});
        } catch (std::bad_function_call &e) {
            Error("m_signal_state_change was unset for " << get_name());
            throw e;
        }
    }

    template<typename NewState>
    void m_change_state_add() {
        try {
            m_signal_state_add(do_state_change_add_overlay{
                .new_state = std::move(std::make_unique<NewState>(m_game, m_registry))});

        } catch (std::bad_function_call &e) {
            Error("m_signal_state_add was unset for " << get_name());
            throw e;
        }
    }
    
  private:
    std::function<void(do_state_change_current)> m_signal_state_change;
    std::function<void(do_state_change_add_overlay)> m_signal_state_add;

    int m_index;
};
