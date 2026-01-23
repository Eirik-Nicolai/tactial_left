#include "rendering.hpp"

void render(entt::registry &reg, olc::PixelGameEngine* ge) {
    std::stringstream ss;
    ge->DrawString(700, 300, "THIS IS A TEXT");
    ge->DrawString(700, 600, "THIS IS A TEXT");

}
