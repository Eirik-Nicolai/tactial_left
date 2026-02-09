#pragma once
#include "olc/olcPixelGameEngine.h"
#include "olc/olcPGEX_TransformedView.h"
#include <entt/entt.hpp>
#include <expected>
#include "states/gamestate.hpp"

#include "logger.hpp"
#include "systems/system.hpp"

constexpr auto MAX_SPRITE_SHEETS = 200;
constexpr auto ANIMATION_TICK_TIME = 0.017;
class TacticalGame : public olc::PixelGameEngine
{
    public:
        TacticalGame();

        int test_x = 0;
        float fElapsedTime;
    public:
        bool OnUserDestroy() override;
        bool OnUserCreate() override;
        bool OnUserUpdate(float) override;

    private: //states
        bool delay_for(float, float);

    public:
        std::vector<std::string> m_debug;

        // bool has_enough_resources(entt::entity&, entt::entity&);
        // std::vector<entt::entity> get_inventory_of_equip_type(entt::entity&);
        // void set_equipment(const entt::entity &);
        // void update_stats_on_hover(entt::entity &);
        // void update_stats_on_select(entt::entity &);

        // template <typename component>
        // void draw_resource_bar(component &c, int x, int y, int w, int h, olc::Pixel col)
        // {
        //     DrawRect(x,y, w, h, col);
        //     FillRect(x+5,y+5,
        //              (w-5)*get_percentage(c.curr, c.max)-5, h-10, olc::WHITE);
        // }

        // void draw_effect_icon(const std::string&, int x, int y, olc::Pixel inner, olc::Pixel outer);

        std::expected<int, std::string> load_decal(const std::string& sprite_path, bool filter, bool clamp) {
            Debug("Loading {} to index {}", sprite_path, m_decals_amount);
            auto sprite = new olc::Sprite();
            if(sprite->LoadFromFile(sprite_path)) {
                //m_decals[m_decals_amount] = std::make_unique<olc::Decal>(sprite, filter, clamp);
                m_decals.push_back(std::make_unique<olc::Decal>(sprite, filter, clamp));
                return m_decals_amount++;
            }
            return std::unexpected<std::string>("Unable to load sprite");
        }

        void unload_decals() {
            Debug("Unloading {} decals", m_decals_amount);

            while(!m_decals.empty())
                m_decals.pop_back();

            m_decals_amount = 0;
        }

        olc::Decal* get_decal(int index) {
            if(index >= m_decals_amount) return nullptr;
            return m_decals[index].get();
        }

        // Spritesheet load_spritesheet(const std::string& sprite_sheet_path,
        //                           int pixel_frame_width,
        //                           int pixel_frame_height) {
        //     Debug("Loading Spritesheet {}", sprite_sheet_path);
        //     if(auto i = load_decal(sprite_sheet_path, false, false); i >= 0) {
        //         //return ;
        //     }
        //     Error("Unable to load spritesheet {}", sprite_sheet_path);
        //     // return -1;
        // }

        // void unload_spritesheet() {
        //     Debug("Unloading {} decals", m_decals_amount);

        //     while(!m_decals.empty())
        //         m_decals.pop_back();

        //     m_decals_amount = 0;
        // }

        bool animation_tick() { return m_animation_tick; }

    public:
        void push_state(GameState* state);
        void pop_state();
        void change_state(GameState* state);

        entt::registry& get_reg() { return m_reg; }
        std::shared_ptr<olc::TileTransformedView> get_tv() { return tvp; }

        std::unique_ptr<olc::Sprite> layer_bg;
        std::unique_ptr<olc::Sprite> layer_main;
        std::unique_ptr<olc::Sprite> layer_wireframe;
        std::unique_ptr<olc::Sprite> layer_gui;

    private:
        std::shared_ptr<olc::TileTransformedView> tvp;
        std::vector<GameState*> m_states;
        entt::registry m_reg;
        std::string get_name() const { return "MAIN ENGINE"; }

        unsigned m_system_managers_amount;
        std::array<std::unique_ptr<SystemManager>, MAX_SYSTEM_AMOUNT> m_system_managers;

        unsigned m_decals_amount;
        std::deque<std::unique_ptr<olc::Decal>> m_decals;

        bool m_animation_tick = false;
        //std::array<olc::Decal*, MAX_SYSTEM_AMOUNT> m_decals;

        float m_fElapsedTime = 0;

    private: //DEBUGGING HELPER FUNCTIONS
};
