#include <iostream>

#include "game.hpp"

auto main(int argc, char *argv[]) -> int {
    TacticalGame game;
    if (game.Construct(1600, 900, 1, 1))
        game.Start();

    return 0;
}
