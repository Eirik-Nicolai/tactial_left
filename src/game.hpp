#pragma once
#include <queue>
#include <entt/entt.hpp>
#include "olcPixelGameEngine.h"

#define OLC_PGE_APPLICATION


#define BORDER_OFFS 10

struct GameState
{
    enum class state
    {
    UNKNOWNTYPE = 0,
    WALKING = 1,
    COMBAT = 2,
    DIALOGUE = 3,
    CUTSCENE = 4,
    PAUSED = 5,
    STARTMENU = 6,
    NONE
};
};

struct GameStateType
{
    enum class type {
    UNKNOWNSUBTYPE = 0,
    INIT = 1,
    PLAYING_TRANSITION = 2,

    // -------- TRANSITIONS --------
    FROM_COMBAT_TRANSITION = 1001,
    FROM_PAUSED_TRANSITION,
    // -------- -------- -------- --

    // ROAMING----------------------
    FREEROAM = 1101,
    // -----------------------------

    // -----------------------------
    PLAYER_SELECTING_ACTION = 2102,
    INIT_COMBAT_ROUND,
    LOAD_ACTIONS,
    LOAD_TARGETS,
    PLAYER_SELECTING_TARGET,
    ENEMY_SELECTING_ACTION,
    ALLY_SELECTING_ACTION,
    PERFORMING_COMBAT_ACTIONS_PLAYER,
    PERFORMING_COMBAT_ACTIONS_ALLIES,
    PERFORMING_COMBAT_ACTIONS_ENEMY,
    PERFORMING_BUFF_DEBUFF_ACTIONS_ALLIES,
    PERFORMING_BUFF_DEBUFF_ACTIONS_ENEMY,
    CLEANUP_COMBAT_TURN,

    // -----------------------------

    // -----------------------------
    OVERVIEW = 5001,
    INIT_INVENTORY,
    INVENTORY,
    EQUIPMENT,
    ITEM_SELECTED,
    // -----------------------------
};
};


using state = GameState::state;
using type = GameStateType::type;

struct State
{
   GameState::state state;
   GameStateType::type type;
};

class TacticalGame : public olc::PixelGameEngine
{
    public:
        TacticalGame();

    public:
        bool OnUserCreate() override;
        bool OnUserUpdate(float) override;

    private: //states
        void STATE_WALKING(float);
        void STATE_COMBAT(float);
        void STATE_PAUSE(float);

        bool delay_for(float, float);

    private:
        entt::registry m_reg;

        State CURR_STATE;
        State NEXT_STATE;

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

    private: //DEBUGGING HELPER FUNCTIONS
};
