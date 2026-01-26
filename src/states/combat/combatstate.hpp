#pragma once
#include "game.hpp"
#include "states/gamestate.hpp"

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

    /// --------- STATE LOGIC --------- ///
    public:
        std::string get_name() final { return "CombatState"; }

        void init(TacticalGame* ge) override;
        void cleanup(TacticalGame* ge) override;

        void enter(TacticalGame* ge) override;
        void exit(TacticalGame* ge) override;

        void handle_input(TacticalGame* ge) override;

        void pause(TacticalGame* ge) override;
        void resume(TacticalGame* ge) override;

        void draw(TacticalGame* ge) override;
        void update(TacticalGame* ge) override; // ?


    /// ------ PRIVATE LOGIC ------ ///
    private:
        float sElapsedTime; // useful for debugging
};
}
