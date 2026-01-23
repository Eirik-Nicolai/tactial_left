#pragma once
#include "game.hpp"
#include "gamestate.hpp"

class InitState : public GameState {
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

        static InitState* get() {
            if(!m_state)
                m_state = new InitState();
            return m_state;
        }
    private:
        static InitState* m_state;
};
