#pragma once
#include "olcPixelGameEngine.h"
#include <entt/entt.hpp>
#include <queue>
#include <print>

#include "states/gamestate.hpp"

#define PRINT_TEXT_NOL(x) std::cout << "[" << get_name() << "::" << __func__ << "()] -->" << " " << x << "\t\t\r";
#define PRINT_FUNC_NOL std::cout << "[" << get_name() << "::" << __func__ << "()]" << "\t\t\r";
#define PRINT_TEXT(x) std::cout << "[" << get_name() << "::" << __func__ << "()] -->" << " " << x << std::endl;
#define PRINT_FUNC std::cout << "[" << get_name() << "::" << __func__ << "()]" << std::endl;

class TacticalGame : public olc::PixelGameEngine
{
    public:
        TacticalGame();

    public:
        bool OnUserCreate() override;
        bool OnUserUpdate(float) override;

    private: //states
        bool delay_for(float, float);

    private:
        entt::registry m_reg;

        std::unique_ptr<GameState> CURR_STATE;
        std::unique_ptr<GameState> NEXT_STATE;

        float m_fElapsedTimeSinceTick;

    public:
        std::vector<std::string> m_debug;
        std::string get_name(const entt::entity&, std::string = "UNNAMED_ENTITY");

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

        template <typename component>
        bool has(entt::entity &ent)
        {
            return m_reg.all_of<component>(ent);
        }

        template <typename component>
        bool tryget_component(entt::entity &ent, component &comp)
        {
            if(auto getcomp = m_reg.try_get<component>(ent);m_reg.try_get<component>(ent) != nullptr)
            {
                comp = (*getcomp);
                return true;
            }
            return false;
        }

        template <typename component>
        bool tryget_component(const entt::entity &ent, component &comp)
        {
            if(auto getcomp = m_reg.try_get<component>(ent);m_reg.try_get<component>(ent) != nullptr)
            {
                comp = (*getcomp);
                return true;
            }
            return false;
        }

        //TODO check reference
        template <typename component>
        component get(entt::entity e)
        {
            return m_reg.get<component>(e);
        }

        template <typename component>
        const component cget(entt::entity e) const
        {
            return m_reg.get<component>(e);
        }

    public:
        void push_state(GameState* state);
        void pop_state();
        void change_state(GameState* state);

    private:
        std::vector<GameState*> m_states;
        std::string get_name() { return "MAIN ENGINE"; }

    private: //DEBUGGING HELPER FUNCTIONS
};
