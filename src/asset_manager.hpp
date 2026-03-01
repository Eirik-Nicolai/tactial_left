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
    // {
    //     Debug("Loading " << sprite_path << " to index " << m_loaded_assets.size());
    //     // TODO check mem leak and better way of loading/unloading sprites
    //     auto sprite = new olc::Sprite();
    //     if (sprite->LoadFromFile(sprite_path)) {
    //         Debug("Loading sprite sheet {" << sprite_path << "}");
    //         m_loaded_assets.emplace_back(friendly_name, olc::Decal(sprite, filter, clamp),
    //                                      AssetType::SPRITESHEET);
    //         // TODO create helper function
    //         std::stringstream ss;
    //         ss << sprite_path << "-";
    //         ss << std::boolalpha << clamp << "-" << filter << "-";
    //         ss << friendly_name;
    //         std::string key = ss.str();
    //         m_name_to_asset_index.insert(std::make_pair(key, m_loaded_assets.size() - 1));
    //         // m_decals[m_decals_amount] =
    //         //     std::make_shared<olc::Decal>(sprite, filter, clamp);
    //         return key;
    //     }
    //     throw std::runtime_error("Unable to load sprite");
    // }

    std::string load_texture(const std::string &sprite_path,
                             std::string friendly_name = "", bool filter = false,
                             bool clamp = false);
    // {
    //     Debug("Loading " << sprite_path << " to index " << m_loaded_assets.size());
    //     // TODO check mem leak and better way of loading/unloading sprites
    //     auto sprite = new olc::Sprite();
    //     if (sprite->LoadFromFile(sprite_path)) {
    //         Debug("Loading sprite sheet {" << sprite_path << "} " << friendly_name);
    //         m_loaded_assets.emplace_back(friendly_name, olc::Decal(sprite, filter, clamp),
    //                                      AssetType::TEXTURE);
    //         // TODO create helper function
    //         std::stringstream ss;
    //         ss << sprite_path << "-";
    //         ss << std::boolalpha << clamp << "-" << filter << "-";
    //         ss << friendly_name;
    //         std::string key = ss.str();
    //         m_name_to_asset_index.insert(std::make_pair(key, m_loaded_assets.size() - 1));
    //         return key;
    //     }
    //     throw std::runtime_error("Unable to load sprite");
    // }

    void unload_decals();
    // {
    //     Debug("Unloading " << m_loaded_assets.size() << " decals");
    //     m_loaded_assets.clear();
    //     m_name_to_asset_index.clear();
    // }   

    std::shared_ptr<olc::Decal> get_texture(const std::string& key);
    std::shared_ptr<olc::Decal> get_spritesheet(const std::string& key);
    // {
    //     if (m_name_to_asset_index.contains(key)) {
    //         Error("Key " << key << "is not tied to a texture");
    //         throw std::runtime_error("Key not tied to loaded texture");
    //     }
    //     return m_loaded_assets[m_name_to_asset_index[key]];
    // }

  private:
    AssetManager();

    // textures are singular image, sprite sheets contain multiple
    std::vector<std::shared_ptr<olc::Decal>> m_loaded_textures;
    std::vector<std::shared_ptr<olc::Decal>> m_loaded_sprite_sheets;
    std::unordered_map<std::string, size_t> m_name_to_asset_index;

    std::string get_name() const { return "Asset Manager"; }
};
