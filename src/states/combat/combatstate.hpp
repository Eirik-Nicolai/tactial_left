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

    virtual void enter(TacticalGame *ge) override;
    virtual void exit(TacticalGame *ge) override {};

    virtual void handle_input(TacticalGame *ge, Engine::Event &) override;

    virtual void pause(TacticalGame *ge) override;
    virtual void resume(TacticalGame *ge) override;

    virtual void draw(TacticalGame *ge) override;
    virtual void update(TacticalGame *ge) override; // ?

    /// ------ PRIVATE LOGIC ------ ///
  protected:
    float sElapsedTime; // useful for debugging
    void solve_a_star(GameRegistry* reg);

    bool mouse_button_released(TacticalGame *ge, Engine::MouseButtonReleasedEvent &event);
    bool mouse_button_pressed(TacticalGame *ge, Engine::MouseButtonPressedEvent &event);

    bool is_panning = false;
};

} // namespace PlayingState
