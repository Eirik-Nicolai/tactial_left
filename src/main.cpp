#include <iostream>

#include "game.hpp"

auto main(int argc, char *argv[]) -> int {

    static constexpr int Height     = 900;
    static constexpr int Width      = 1600;

    static constexpr int PixelH     = 1;
    static constexpr int PixelW     = 1;

    static constexpr bool Fullscreen = false;
    static constexpr bool V_sync     = true;
    static constexpr bool Cohesion   = true;
    static constexpr bool RealWindow = true;

    TacticalGame game;
    if (game.Construct(Width, Height, PixelH, PixelW, Fullscreen, V_sync, Cohesion, RealWindow))
        game.Start();

    return 0;
}
