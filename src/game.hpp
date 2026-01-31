#pragma once
#include "olc/olcPixelGameEngine.h"
#include "olc/olcPGEX_TransformedView.h"
#include <entt/entt.hpp>
#include <queue>

#include "states/gamestate.hpp"

#include "logger.hpp"
#include "systems/system.hpp"

class TacticalGame : public olc::PixelGameEngine
{
    public:
        TacticalGame();

    public:
        bool OnUserDestroy() override;
        bool OnUserCreate() override;
        bool OnUserUpdate(float) override;

    private: //states
        bool delay_for(float, float);

    public:
        std::vector<std::string> m_debug;

        bool has_enough_resources(entt::entity&, entt::entity&);
        std::vector<entt::entity> get_inventory_of_equip_type(entt::entity&);
        void set_equipment(const entt::entity &);
        void update_stats_on_hover(entt::entity &);
        void update_stats_on_select(entt::entity &);

        template <typename component>
        void draw_resource_bar(component &c, int x, int y, int w, int h, olc::Pixel col)
        {
            DrawRect(x,y, w, h, col);
            FillRect(x+5,y+5,
                     (w-5)*get_percentage(c.curr, c.max)-5, h-10, olc::WHITE);
        }

        void draw_effect_icon(const std::string&, int x, int y, olc::Pixel inner, olc::Pixel outer);

    public:
        void push_state(GameState* state);
        void pop_state();
        void change_state(GameState* state);

        entt::registry& get_reg() { return m_reg; }
        std::shared_ptr<olc::TileTransformedView> get_tv() { return tvp; }

    private:
        std::shared_ptr<olc::TileTransformedView> tvp;
        std::vector<GameState*> m_states;
        entt::registry m_reg;
        std::string get_name() const { return "MAIN ENGINE"; }

        unsigned m_system_managers_amount;
        std::array<std::unique_ptr<SystemManager>, MAX_SYSTEM_AMOUNT> m_system_managers;

    private: //DEBUGGING HELPER FUNCTIONS
};
