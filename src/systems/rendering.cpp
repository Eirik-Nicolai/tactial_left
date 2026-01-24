#include "rendering.hpp"

#include "components/components.hpp"
#include "utils/geometry.hpp"
#include "states/star/starstate.hpp"

#include "utils/ecs.hpp"

void draw_planets(TacticalGame* ge) {
    auto &reg = ge->get_reg();
    auto tv = ge->get_tv();

    auto get_name = [](){
        return "draw_planets";
    };

    for(auto [ent, pos, orb] : reg.view<Pos, Orbiting>().each())
    {
        auto rot_centre = get<Pos>(reg, orb.anchor).coordinates;

        tv->DrawLine(rot_centre, pos.coordinates, olc::CYAN, 0xF0F0F0F0);
    }

    for(auto [ent, pos, wireframe, circular] : reg.view<Pos, Rendering::Wireframe, SizeCirc>().each())
    {
        tv->DrawCircle(pos.coordinates, circular.r, wireframe.color);
    }
}

void State::Star::render_stars(entt::registry &reg, TacticalGame* ge) {

    static int offs = 50;
    static int w = 80;
    static int h = 50;

    draw_planets(ge);

    for(auto [ent, pos, circular] : reg.view<Pos, SizeCirc, Tag::Hovered>().each()) {
        auto tv = ge->get_tv();
        tv->FillCircle(pos.coordinates, circular.r*0.75, olc::WHITE);
    }
}
