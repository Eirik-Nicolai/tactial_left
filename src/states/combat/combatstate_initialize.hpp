#pragma once
#include "combatstate.hpp"
#include "game.hpp"
#include "logger.hpp"
#include "engine/event.hpp"
#include "engine/key_event.hpp"
namespace TransitionState {
class CombatStateInit : public PlayingState::CombatState
{
    GET_NAME(GameState, CombatStateInit)

    /// --------- STATE LOGIC --------- ///
  public:
    CombatStateInit(TacticalGame* ge, std::shared_ptr<GameRegistry> reg);
    ~CombatStateInit() {LOG_FUNC};

    void handle_input(Engine::Event &) final;
    
    void pause() override {};
    void resume() override {};

    void draw() override {};
    void update() override; // ?
                                            /// ------ PRIVATE LOGIC ------ ///
  private:
    bool key_released(Engine::KeyReleasedEvent &event);
};
};
