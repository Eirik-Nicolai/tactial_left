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
    CombatState(TacticalGame*, std::shared_ptr<GameRegistry> registry);
    ~CombatState();

    virtual void handle_input(Engine::Event &) override;

    virtual void pause() override;
    virtual void resume() override;

    virtual void draw(TacticalGame *ge) override;
    virtual void update() override; // ?

    /// ------ PRIVATE LOGIC ------ ///
  protected:
    float sElapsedTime; // useful for debugging
    void solve_a_star(std::shared_ptr<GameRegistry> reg);

    bool mouse_button_released(Engine::MouseButtonReleasedEvent &event);
    bool mouse_button_pressed(Engine::MouseButtonPressedEvent &event);

    int screen_w;
    int screen_h;
    int rect_w;
    int rect_h;
};

} // namespace PlayingState
