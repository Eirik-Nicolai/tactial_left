#pragma once
#include "components.hpp"
namespace Component::Interaction
{
Component(Hoverable)
    Box boundaries;
    bool is_hovered;
    EcsCallback on_mouse_hover;
    EcsCallback on_mouse_exit;
};

Component(Selectable)
    EcsCallback on_left_select;
    EcsCallback on_middle_select;
    EcsCallback on_right_select;
};
}; // namespace Interaction
