#pragma once
#include "game.hpp"
#include "states/gamestate.hpp"

namespace PlayingState {
class InitState : public GameState {
    /// ------ SINGLETON LOGIC ------ ///
    public:
        static InitState* Instance() {
            if(!m_state)
                m_state = new InitState();
            return m_state;
        }
    private:
        static InitState* m_state;

    /// --------- STATE LOGIC --------- ///
    public:
        std::string get_name() { return "InitState"; }

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
