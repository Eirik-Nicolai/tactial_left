#ifndef RENDERING_H_
#define RENDERING_H_

#include "components.hpp"

namespace Component::Rendering
{
namespace Layer
{
Tag(pre)
Tag(bg_far)
Tag(bg_near)
Tag(furthest)
Tag(middle)
Tag(closest)
Tag(post)
Tag(gui)
Tag(wireframe)
}; // namespace Layer

namespace Terrain
{

};

Component(Spritesheet)
    unsigned decal_index;
    Size pixel_frame_size;
};

Component(RenderingManager)

    // entt::entity sprite_sheet;
    olc::vf2d pos_sprite_sheet;
    olc::vf2d sprite_scale;
};

namespace GUI
{
};
Component(Wireframe)
    enum class TYPE {
        CIRCLE,
        SQUARE,
        TRIANGLE,
        CIRCLE_FILL,
        SQUARE_FILL,
        TRIANGLE_FILL,
    } type;
    olc::Pixel color;
};

}; // namespace Rendering

#endif // RENDERING_H_
