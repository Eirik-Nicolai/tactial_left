#pragma once
#include "olc/olcPixelGameEngine.h"
#include "components/components.hpp"

inline bool is_point_inside_circle(int circle_x, int circle_y, int rad, int x, int y)
{
    // Compare radius of circle with distance
    // of its center from given point
    if ((x - circle_x) * (x - circle_x) + (y - circle_y) * (y - circle_y) <= rad * rad) {
        return true;
    }
    return false;
}

inline bool is_point_inside_rect(int rect_x, int rect_y, int rect_w, int rect_h, int px,
                                 int py)
{
    // Compare radius of circle with distance
    // of its center from given point
    if (px >= rect_x && px <= rect_x + rect_w && py >= rect_y && py <= rect_y + rect_h) {
        return true;
    }
    return false;
}

template <typename Position, typename Size, typename OtherPosition>
inline bool is_point_inside_rect(Position rect_pos, Size rect_size,
                                 OtherPosition mouse_pos)
{
    return is_point_inside_rect(rect_pos.x, rect_pos.y, rect_size.w, rect_size.h,
                                mouse_pos.x, mouse_pos.y);
}
