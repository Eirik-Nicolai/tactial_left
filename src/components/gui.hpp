#ifndef GUI_H_
#define GUI_H_
#include "olc/olcPixelGameEngine.h"

// size and pos in screenspace not game space
// mostly for gui ?
// probably not needed
namespace GUI
{
struct _container {
};
struct Text {
    std::string value;
};
struct Size {
    float h;
    float w;
    operator olc::vf2d() const { return olc::vf2d(h, w); };
    operator olc::vi2d() const { return olc::vi2d(h, w); };
};
struct Pos {
    float x;
    float y;
    operator olc::vf2d() const { return olc::vf2d(x, y); };
    operator olc::vi2d() const { return olc::vi2d(x, y); };
};
} // namespace GUI

#endif // GUI_H_
