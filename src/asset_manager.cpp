#include "asset_manager.hpp"
#include "logger.hpp"

void split (std::stringstream &output, const std::string &s, char delim) {
    std::stringstream ss (s);
    std::string item;

    while (getline (ss, item, delim)) {
        output << item;
    }
}

AssetManager::AssetManager() {
    Info("Initiated asset manager");
}

std::string AssetManager::load_sprite_sheet(const std::string &sprite_path,
                                            std::string friendly_name, bool filter,
                                            bool clamp)
{
    Debug("Loading " << sprite_path << " to index " << m_loaded_sprite_sheets.size());
    // TODO check mem leak and better way of loading/unloading sprites
    auto sprite = new olc::Sprite();
    if (sprite->LoadFromFile(sprite_path) == olc::rcode::OK) {
        Debug("Loading sprite sheet {" << sprite_path << "}");
        m_loaded_sprite_sheets.emplace_back(
            std::make_shared<olc::Decal>(sprite, filter, clamp));
        // TODO create helper function
        std::stringstream ss;
        ss << sprite_path << "-";
        ss << std::boolalpha << clamp << "-" << filter << "-";
        split(ss, friendly_name, ' ');
        std::string key = ss.str();
        Info("Created index " << key);
        m_name_to_asset_index.insert(std::make_pair(key, m_loaded_sprite_sheets.size() - 1));
        return key;
    }
    throw std::runtime_error("Unable to load sprite");
}

std::string AssetManager::load_texture(const std::string &sprite_path,
                                       std::string friendly_name, bool filter, bool clamp)
{
    Debug("Loading " << sprite_path << " to index " << m_loaded_textures.size());
    // TODO check mem leak and better way of loading/unloading sprites
    auto sprite = new olc::Sprite();
    if (sprite->LoadFromFile(sprite_path) == olc::rcode::OK) {
        Debug("Loading sprite sheet {" << sprite_path << "} " << friendly_name);
        m_loaded_textures.emplace_back(
            std::make_shared<olc::Decal>(sprite, filter, clamp));
        // TODO create helper function
        std::stringstream ss;
        ss << sprite_path << "-";
        ss << std::boolalpha << clamp << "-" << filter << "-";
        split(ss, friendly_name, ' ');
        std::string key = ss.str();
        m_name_to_asset_index.insert(std::make_pair(key, m_loaded_textures.size() - 1));
        return key;
    }
    throw std::runtime_error("Unable to load sprite");
}

void AssetManager::unload_decals()
{
    Warn("Function unload_decals not implemented");
    // Debug("Unloading " << m_loaded_assets.size() << " decals");
    m_loaded_textures.clear();
    m_name_to_asset_index.clear();
}

std::shared_ptr<olc::Decal> AssetManager::get_spritesheet(const std::string& key)
{
    if (!m_name_to_asset_index.contains(key)) {
        Error("Key " << key << "is not tied to a texture");
        throw std::runtime_error("Key not tied to loaded texture");
    }
    return m_loaded_sprite_sheets[m_name_to_asset_index[key]];
}

std::shared_ptr<olc::Decal> AssetManager::get_texture(const std::string& key)
{
    if (!m_name_to_asset_index.contains(key)) {
        Error("Key " << key << "is not tied to a texture");
        throw std::runtime_error("Key not tied to loaded texture");
    }
    return m_loaded_textures[m_name_to_asset_index[key]];
}

std::pair<int, int> AssetManager::get_texture_size(const std::string& key) {
    auto d = get_texture(key);
    return std::make_pair(d->width, d->height);
}
