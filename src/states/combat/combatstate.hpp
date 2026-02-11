#pragma once
#include "game.hpp"
#include "states/gamestate.hpp"
#include "utils/complex_datatypes.hpp"

constexpr auto tile_amt_x = 20;
constexpr auto tile_amt_y = 9;

namespace PlayingState {
class CombatState : public GameState {
    /// ------ SINGLETON LOGIC ------ ///
    public:
        static CombatState* Instance() {
            if(!m_state)
                m_state = new CombatState();
            return m_state;
        }
    private:
        static CombatState* m_state;


    GET_NAME(GameState, Combatstate)
    /// --------- STATE LOGIC --------- ///
    public:

        void init(TacticalGame* ge) override;
        void cleanup(TacticalGame* ge) override;

        void enter(TacticalGame* ge) override;
        void exit(TacticalGame* ge) override;

        void handle_input(TacticalGame* ge) override;

        void pause(TacticalGame* ge) override;
        void resume(TacticalGame* ge) override;

        void draw(TacticalGame* ge) override;
        void update(TacticalGame* ge) override; // ?

        void solve_a_star();

    /// ------ PRIVATE LOGIC ------ ///
    private:
        float sElapsedTime; // useful for debugging

        std::array<std::shared_ptr<Node>, tile_amt_x*tile_amt_y> combat_tiles;
        std::shared_ptr<Node> node_start = nullptr;
        std::shared_ptr<Node> node_end = nullptr;

};
}
