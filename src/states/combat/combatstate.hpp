#pragma once
#include "game.hpp"
#include "states/gamestate.hpp"
#include "engine/event.hpp"
#include "engine/mouse_event.hpp"
#include "engine/input.hpp"

#include "components/components.hpp"
#include "components/combat.hpp"
#include "components/interaction.hpp"
#include "components/rendering.hpp"
#include "components/animation.hpp"

constexpr auto tile_amt_x = 20;
constexpr auto tile_amt_y = 9;
namespace PlayingState
{
class CombatState : public GameState
{
    GET_NAME(GameState, Combatstate)

    /// --------- STATE LOGIC --------- ///
  public:
    CombatState();
    ~CombatState();

    void enter(TacticalGame *ge) override;
    void exit(TacticalGame *ge) override;

    void handle_input(TacticalGame *ge, Engine::Event &) override;

    void pause(TacticalGame *ge) override;
    void resume(TacticalGame *ge) override;

    void draw(TacticalGame *ge) override;
    void update(TacticalGame *ge) override; // ?

    /// ------ PRIVATE LOGIC ------ ///
  protected:
    float sElapsedTime; // useful for debugging
    void solve_a_star(GameRegistry* reg);

    bool mouse_button_released(TacticalGame *ge, MouseButtonReleasedEvent &event);
    bool mouse_button_pressed(TacticalGame *ge, MouseButtonPressedEvent &event);

    bool is_panning = false;
};

class CombatStatePlayerMovement : public CombatState
{
    GET_NAME(GameState, Combatstate)

    /// --------- STATE LOGIC --------- ///
  public:
    CombatStatePlayerMovement() = default;
    ~CombatStatePlayerMovement() = default;

    void on_input(TacticalGame *ge,Input &);

    void draw(TacticalGame *ge) override;
    void update(TacticalGame *ge) override; // ?
    /// ------ PRIVATE LOGIC ------ ///
  private:
    bool mouse_button_released(TacticalGame *ge, MouseButtonReleasedEvent &event);
    bool mouse_button_pressed(TacticalGame *ge, MouseButtonPressedEvent &event);
};
   
class CombatStatePlayerAction : public CombatState
{
    GET_NAME(GameState, Combatstate)

    /// --------- STATE LOGIC --------- ///
  public:
    CombatStatePlayerAction() = default;
    ~CombatStatePlayerAction() = default;

    void on_input(TacticalGame *ge,Input &);
    
    void draw(TacticalGame *ge) override;
    void update(TacticalGame *ge) override; // ?
    /// ------ PRIVATE LOGIC ------ ///
  private:
    bool mouse_button_released(TacticalGame *ge, MouseButtonReleasedEvent &event);
    bool mouse_button_pressed(TacticalGame *ge, MouseButtonPressedEvent &event);
};
       
class CombatStateDoCombat : public CombatState
{
    GET_NAME(GameState, Combatstate)

    /// --------- STATE LOGIC --------- ///
  public:
    CombatStateDoCombat() = default;
    ~CombatStateDoCombat() = default;

    void on_input(TacticalGame *ge,Input &);

    void draw(TacticalGame *ge) override;
    void update(TacticalGame *ge) override; // ?
    /// ------ PRIVATE LOGIC ------ ///
  private:
    bool mouse_button_released(TacticalGame *ge, MouseButtonReleasedEvent &event);
    bool mouse_button_pressed(TacticalGame *ge, MouseButtonPressedEvent &event);
};

} // namespace PlayingState
