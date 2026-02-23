#pragma once
#include "combatstate.hpp"
#include "game.hpp"

namespace PlayingState {

class CombatStatePlayerMovement : public CombatState
{
    GET_NAME(GameState, Combatstate)

    /// --------- STATE LOGIC --------- ///
  public:
    CombatStatePlayerMovement();
    ~CombatStatePlayerMovement() = default;
    
    void enter(TacticalGame *ge) override {};
    void exit(TacticalGame *ge) override {};

    void handle_input(TacticalGame *ge, Engine::Event &) final {};
    void on_input(TacticalGame *ge, Input &);

    void pause(TacticalGame *ge) override {};
    void resume(TacticalGame *ge) override {};

    void draw(TacticalGame *ge) override;
    void update(TacticalGame *ge) override; // ?
    /// ------ PRIVATE LOGIC ------ ///
  private:
    bool mouse_button_released(TacticalGame *ge, Engine::MouseButtonReleasedEvent &event);
    bool mouse_button_pressed(TacticalGame *ge, Engine::MouseButtonPressedEvent &event);
    using InputFunction = std::function<void(TacticalGame*, Input*)>;
    std::map<std::string,InputFunction> m_inputs;
};

class CombatStatePlayerAction : public CombatState
{
    GET_NAME(GameState, Combatstate)

    /// --------- STATE LOGIC --------- ///
  public:
    CombatStatePlayerAction() = default;
    ~CombatStatePlayerAction() = default;

    void on_input(TacticalGame *ge, Input &);

    void draw(TacticalGame *ge) override;
    void update(TacticalGame *ge) override; // ?
                                            /// ------ PRIVATE LOGIC ------ ///
  private:
    bool mouse_button_released(TacticalGame *ge, Engine::MouseButtonReleasedEvent &event);
    bool mouse_button_pressed(TacticalGame *ge, Engine::MouseButtonPressedEvent &event);
};

class CombatStateDoCombat : public CombatState
{
    GET_NAME(GameState, Combatstate)

    /// --------- STATE LOGIC --------- ///
  public:
    CombatStateDoCombat() = default;
    ~CombatStateDoCombat() = default;

    void on_input(TacticalGame *ge, Input &);

    void draw(TacticalGame *ge) override;
    void update(TacticalGame *ge) override; // ?
                                            /// ------ PRIVATE LOGIC ------ ///
  private:
    bool mouse_button_released(TacticalGame *ge, Engine::MouseButtonReleasedEvent &event);
    bool mouse_button_pressed(TacticalGame *ge, Engine::MouseButtonPressedEvent &event);
};

}; // namespace PlayingState
