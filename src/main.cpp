#include <iostream>

#include "game.hpp"

// TODO move to own file/class
#include <fstream>
#include <fkYAML/node.hpp>

auto main(int argc, char *argv[]) -> int
{

    std::cout << "Initializing game ..." << std::endl;
    ;

    std::cout << "READ CONFIG.." << std::endl;

    // TODO move all this logic to config reading
    // class
    std::fstream fs;
    fs.open("config.yml");
    if (!fs.is_open()) {
        // Error("NO CONFIG FILE");
        throw std::runtime_error("Unable to find config");
    }

    int Height = 1080;
    int Width = 1920;
    int PixelH = 1;
    int PixelW = 1;
    bool Fullscreen = false;
    bool V_sync = false;
    bool Cohesion = false;
    bool RealWindow = true;

    auto conf = fkyaml::node::deserialize(fs);
    try {
        // TODO can cast string to enum instead
        auto conf_level = [&](std::string &conf) -> int {
            if (conf == "TRACE") {
                return 0;
            }
            if (conf == "DEBUG") {
                return 1;
            }
            if (conf == "INFO") {
                return 2;
            }
            throw std::runtime_error("Throw");
        };

        auto level = conf.at("log_level").as_str();
        std::cout << "Level is " << level << std::endl;
        // Logger::Get()->set_log_level(
        //     static_cast<spdlog::level::level_enum>(conf_level(level)));

        Height = conf.at("screenheight").as_int();
        Width = conf.at("screenwidth").as_int();
        PixelH = conf.at("pixelwidth").as_int();
        PixelW = conf.at("pixelheight").as_int();
        Fullscreen = conf.at("fullscreen").as_bool();
        V_sync = conf.at("vsync").as_bool();
        Cohesion = conf.at("cohesion").as_bool();
        RealWindow = conf.at("realwindow").as_bool();
    } catch (std::exception &e) {
        std::cout << "ERROR " << e.what() << std::endl;
        throw std::runtime_error("CANNOT READ CONFIG");
    }

    TacticalGame game;
    if (game.Construct(Width, Height, PixelH, PixelW, Fullscreen, V_sync, Cohesion,
                       RealWindow))
        game.Start();

    return 0;
}
