#pragma once
#include "combatstate.hpp"
#include "game.hpp"

namespace PlayingState {

class CombatStatePlayerMovement : public CombatState
{
    GET_NAME(GameState, CombatstateMovement)

    /// --------- STATE LOGIC --------- ///
  public:
    CombatStatePlayerMovement(TacticalGame*, std::shared_ptr<GameRegistry>);
    ~CombatStatePlayerMovement() = default;
    
    void handle_input(Engine::Event &) final;
    
    void pause() override {};
    void resume() override {};

    void draw(TacticalGame*) override;
    void update() override; // ?
    /// ------ PRIVATE LOGIC ------ ///
  private:
    bool mouse_button_moved(Engine::MouseMovedEvent &event);
    bool mouse_button_released(Engine::MouseButtonReleasedEvent &event);
    bool mouse_button_pressed(Engine::MouseButtonPressedEvent &event);
    using InputFunction = std::function<void(TacticalGame*, Engine::GameEvent&)>;
    std::map<std::string_view,InputFunction> m_inputs;

    entt::entity m_controlled_entity;
    entt::entity m_tile_dest;
};

class CombatStatePlayerAction : public CombatState
{
    GET_NAME(GameState, Combatstate)

    /// --------- STATE LOGIC --------- ///
  public:
    CombatStatePlayerAction(TacticalGame* ge, std::shared_ptr<GameRegistry> reg) : CombatState(ge, reg) {};
    ~CombatStatePlayerAction() = default;

    void handle_input(Engine::Event &) final {};
    
    void pause() override {};
    void resume() override {};

    void draw(TacticalGame*) override;
    void update() override; // ?
                                            /// ------ PRIVATE LOGIC ------ ///
  private:
    bool mouse_button_released(Engine::MouseButtonReleasedEvent &event) {return true;};
    bool mouse_button_pressed(Engine::MouseButtonPressedEvent &event) {return true;};
};

// class CombatStateDoCombat : public CombatState
// {
//     GET_NAME(GameState, Combatstate)

//     /// --------- STATE LOGIC --------- ///
//   public:
//     CombatStateDoCombat() = default;
//     ~CombatStateDoCombat() = default;

//     void on_input(, Input &);

//     void draw() override;
//     void update() override; // ?
//                                             /// ------ PRIVATE LOGIC ------ ///
//   private:
//     bool mouse_button_released(, Engine::MouseButtonReleasedEvent &event);
//     bool mouse_button_pressed(, Engine::MouseButtonPressedEvent &event);
// };

}; // namespace PlayingState
