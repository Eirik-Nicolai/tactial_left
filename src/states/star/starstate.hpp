#pragma once
#include "game.hpp"
#include "states/gamestate.hpp"
#include "utils/geometry.hpp"
namespace PlayingState {
class StarState : public GameState {
    /// ------ SINGLETON LOGIC ------ ///
    public:
        static StarState* Instance() {
            if(!m_state)
                m_state = new StarState();
            return m_state;
        }
    private:
        static StarState* m_state;


    /// --------- STATE LOGIC --------- ///
    public:
        std::string get_name() const override { return "StarState"; }

        void init(TacticalGame* ge) override;
        void cleanup(TacticalGame* ge) override;

        void enter(TacticalGame* ge) override;
        void exit(TacticalGame* ge) override;

        void handle_input(TacticalGame* ge) override;

        void pause(TacticalGame* ge) override;
        void resume(TacticalGame* ge) override;

        void draw(TacticalGame* ge) override;
        void update(TacticalGame* ge) override; // ?
};

class StarStateSelected : public StarState {
        /// ------ SINGLETON LOGIC ------ ///
    public:
        static StarStateSelected* Instance() {
            if(!m_state)
                m_state = new StarStateSelected();
            return m_state;
        }
    private:
        static StarStateSelected* m_state;


        /// --------- STATE LOGIC --------- ///
    public:
        std::string get_name() const final { return "StarState - Selected"; }

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
    public:
        void set_camera_point(entt::entity& ett) {
            pointofinterest = ett;
        }

    private:
        float sElapsedTime; // useful for debugging

        olc::vf2d get_mouse_pos_screen(TacticalGame* ge) {
            return ge->get_tv()->ScaleToWorld(ge->GetMousePos()) + ge->get_tv()->GetWorldOffset();
        }

        entt::entity pointofinterest;
};

}
