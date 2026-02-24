#ifndef INTERACTION_H_
#define INTERACTION_H_
#include "components.hpp"
namespace Interaction
{
Component(Hoverable)
    bool is_hovered;
};
Component(Selectable)
    bool is_selected;
};

}; // namespace Interaction

#endif // INTERACTION_H_
