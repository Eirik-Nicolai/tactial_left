#pragma once
#include "combatstate.hpp"
#include "game.hpp"

namespace PlayingState {
  
class CombatStatePlayerAction : public CombatState
{
    GET_NAME(GameState, CombatStatePlayerAction)

    /// --------- STATE LOGIC --------- ///
  public:
    CombatStatePlayerAction(TacticalGame* ge, std::shared_ptr<GameRegistry> reg) : CombatState(ge, reg) {};
    ~CombatStatePlayerAction() {LOG_FUNC};

    void handle_input(Engine::Event &) final {};
    
    void pause() override {};
    void resume() override {};

    void draw() override;
    void update() override; // ?
                                            /// ------ PRIVATE LOGIC ------ ///
  private:
    bool mouse_button_released(Engine::MouseButtonReleasedEvent &event) {return true;};
    bool mouse_button_pressed(Engine::MouseButtonPressedEvent &event) {return true;};
};

};
