#pragma once
#include "olc/olcPixelGameEngine.h"


constexpr auto MAX_SPRITE_SHEETS = 200;

enum class AssetType { NONETYPE = 0, SPRITESHEET, TEXTURE };

// struct Asset {
//     Asset(std::string s, olc::Decal dec, AssetType t) : title(s), decal(dec), type(t) {}
//     Asset(const Asset &other) : title(other.title), decal(other.decal), type(other.type)
//     {}
//     Asset(const Asset &&other) : title(other.title), decal(other.decal), type(other.type)
//     {}
//     std::string title;
//     olc::Decal decal;
//     AssetType type;
// };

class AssetManager
{

  public:
    // SINGLETON LOGIC
    //
    static AssetManager &instance()
    {
        // The logger variable is initialized once with a move assignment of a logger
        // object build with the private constructor.
        static auto &&mng = AssetManager();
        return (mng);
    }

  public:
    // TODO can template this in the future ?
    std::string load_sprite_sheet(const std::string &sprite_path,
                                  std::string friendly_name = "", bool filter = false,
                                  bool clamp = false);

    std::string load_texture(const std::string &sprite_path,
                             std::string friendly_name = "", bool filter = false,
                             bool clamp = false);

    std::pair<int, int> get_texture_size(const std::string& key);
    void unload_decals();

    std::shared_ptr<olc::Decal> get_texture(const std::string& key);
    std::shared_ptr<olc::Decal> get_spritesheet(const std::string& key);

  private:
    AssetManager();

    // textures are singular image, sprite sheets contain multiple
    std::vector<std::shared_ptr<olc::Decal>> m_loaded_textures;
    std::vector<std::shared_ptr<olc::Decal>> m_loaded_sprite_sheets;
    std::unordered_map<std::string, size_t> m_name_to_asset_index;

    std::string get_name() const { return "Asset Manager"; }
};
