#pragma once
#include "olc/olcPixelGameEngine.h"
#include "olc/olcPGEX_TransformedView.h"
#include <entt/entt.hpp>
#include <expected>

#include "logger.hpp"
#include "layers/layer.hpp"
#include "systems/system.hpp"

class Layer;

constexpr auto MAX_SPRITE_SHEETS = 200;
constexpr auto ANIMATION_TICK_TIME = 0.017;
class TacticalGame : public olc::PixelGameEngine
{
    public:
        TacticalGame();

        float fElapsedTime;
    public:
        bool OnUserDestroy() override;
        bool OnUserCreate() override;
        bool OnUserUpdate(float) override;

    public:
        std::expected<int, std::string> load_decal(const std::string& sprite_path, bool filter, bool clamp) {
            Debug("Loading {} to index {}", sprite_path, m_decals_amount);
            auto sprite = new olc::Sprite();
            if(sprite->LoadFromFile(sprite_path)) {
                m_decals[m_decals_amount] = std::make_shared<olc::Decal>(sprite, filter, clamp);
                return m_decals_amount++;
            }
            return std::unexpected<std::string>("Unable to load sprite");
        }

        void unload_decals() {
            Debug("Unloading {} decals", m_decals_amount);

            while(m_decals_amount != 0)
                m_decals[m_decals_amount--].reset();
        }

        std::shared_ptr<olc::Decal> get_decal(int index) {
            if(index >= m_decals_amount) return nullptr;
            return m_decals[index];
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

        void raise_event(Event& event);

    template <typename L>
    requires(std::is_base_of_v<Layer, L>)
    void add_layer() {
        m_layers.push_back(std::make_unique<L>(this));
    }
    
    public:
        // void push_state(GameState* state);
        // void pop_state();
        // void change_state(GameState* state);

        entt::registry& get_reg() { return m_reg; }
        std::shared_ptr<olc::TileTransformedView> get_tv() { return tvp; }

        std::unique_ptr<olc::Sprite> layer_bg;
        std::unique_ptr<olc::Sprite> layer_main;
        std::unique_ptr<olc::Sprite> layer_wireframe;
        std::unique_ptr<olc::Sprite> layer_gui;

    private:
        std::shared_ptr<olc::TileTransformedView> tvp;
        std::vector<std::unique_ptr<Layer>> m_layers;
        entt::registry m_reg;
        std::string get_name() const { return "MAIN ENGINE"; }

        unsigned m_system_managers_amount;
        std::array<std::unique_ptr<SystemManager>, MAX_SYSTEM_AMOUNT> m_system_managers;

        unsigned m_decals_amount;
        std::array<std::shared_ptr<olc::Decal>, MAX_SPRITE_SHEETS> m_decals;
        // std::deque<std::unique_ptr<olc::Decal>> m_decals;

        bool m_animation_tick = false;

        float m_fElapsedTime = 0;

    private: //DEBUGGING HELPER FUNCTIONS
};
