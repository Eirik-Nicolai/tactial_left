#pragma once

#include "components.hpp"

// size and pos in screenspace not game space
// mostly for gui ?
// probably not needed
class GameRegistry;
namespace GUI
{
Tag(container);

Tag(is_gui);
    
Component(Button)
    std::string text; // unneeded ?
    std::function<void(std::shared_ptr<GameRegistry>)> handler;
};
Component(Text)
    std::string value;
};
Component(Size)
    float h;
    float w;
    operator olc::vf2d() const { return olc::vf2d(h, w); };
    operator olc::vi2d() const { return olc::vi2d(h, w); };
};
Component(Pos)
    float x;
    float y;
    operator olc::vf2d() const { return olc::vf2d(x, y); };
    operator olc::vi2d() const { return olc::vi2d(x, y); };
};
} // namespace GUI
