#ifndef RENDERING_H_
#define RENDERING_H_

#include "components.hpp"

namespace Rendering
{
namespace Layer
{
struct _pre {
};
struct _bg_far {
};
struct _bg_near {
};
struct _furthest {
};
struct _middle {
};
struct _closest {
};
struct _post {
};
struct _gui {
};
struct _wireframe {
};
}; // namespace Layer

namespace Terrain
{

};

struct Spritesheet {
    unsigned decal_index;
    Size pixel_frame_size;
};

struct RenderingManager {
    // entt::entity sprite_sheet;
    olc::vf2d pos_sprite_sheet;
    olc::vf2d sprite_scale;
};

namespace GUI
{
};
struct Wireframe {
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
