#include "combatstate_playermovement.hpp"
#include "logger.hpp"
namespace PlayingState
{

CombatStatePlayerMovement::CombatStatePlayerMovement() {
    handler = std::make_unique<InputHandler>();
    // handler->register_input("TEST_INPUT", [this](TacticalGame* ge, Input* inp){
    //     Info("Received input " << inp->get_name());
    // });
    m_inputs["TEST_INPUT"] = [](TacticalGame *ge, Engine::GameEvent &inp) {
        auto get_name = []() { return "INPUT CALLBACK ------"; };
        Info("Received input " << inp.get_name());
    };
}

void CombatStatePlayerMovement::handle_input(TacticalGame *ge, Engine::GameEvent &input)
{
    Info("Received input " << input);
    m_inputs[input.get_name()](ge, input);
}

void CombatStatePlayerMovement::on_input(TacticalGame *ge, Input &input)
{
    // Error("Got input " << event);
    // Engine::EventDispatcher dispatcher(ge, event);
    // dispatcher.Dispatch<Engine::MouseButtonPressedEvent>(
    //     [this](TacticalGame *ge, Engine::MouseButtonPressedEvent &e) {
    //         return mouse_button_pressed(ge, e);
    //     });
    // dispatcher.Dispatch<Engine::MouseButtonReleasedEvent>(
    //     [this](TacticalGame *ge, Engine::MouseButtonReleasedEvent &e) {
    //         return mouse_button_released(ge, e);
    //     });
    Info("Received input");
    //m_inputs[input.get_name()](ge, &input);
}

void CombatStatePlayerMovement::update(TacticalGame *ge)
{
    // Error("Got input " << event);
    // Engine::EventDispatcher dispatcher(ge, event);
    // dispatcher.Dispatch<Engine::MouseButtonPressedEvent>(
    //     [this](TacticalGame *ge, Engine::MouseButtonPressedEvent &e) {
    //         return mouse_button_pressed(ge, e);
    //     });
    // dispatcher.Dispatch<Engine::MouseButtonReleasedEvent>(
    //     [this](TacticalGame *ge, Engine::MouseButtonReleasedEvent &e) {
    //         return mouse_button_released(ge, e);
    //     });
    Info("Received input");
}

void CombatStatePlayerMovement::draw(TacticalGame *ge)
{
    // Error("Got input " << event);
    // Engine::EventDispatcher dispatcher(ge, event);
    // dispatcher.Dispatch<Engine::MouseButtonPressedEvent>(
    //     [this](TacticalGame *ge, Engine::MouseButtonPressedEvent &e) {
    //         return mouse_button_pressed(ge, e);
    //     });
    // dispatcher.Dispatch<Engine::MouseButtonReleasedEvent>(
    //     [this](TacticalGame *ge, Engine::MouseButtonReleasedEvent &e) {
    //         return mouse_button_released(ge, e);
    //     });
    Info("Received input");
}

// bool CombatStatePlayerMovement::mouse_button_released(TacticalGame *ge,
// MouseButtonReleasedEvent &event)
// {
//     auto get_name = []() { return "CombatSelect - mouse_button_released()"; };
//     Debug("Released");
//     return false;
// }
// bool CombatStatePlayerMovement::mouse_button_pressed(TacticalGame *ge,
// MouseButtonPressedEvent &event)
// {
//     auto get_name = []() { return "CombatSelect - mouse_button_pressed()"; };
//     // HACK
//     Debug("Mouse " << event);
//     auto sw = ge->ScreenWidth();
//     auto sh = ge->ScreenHeight();
//     auto w = (0.04);
//     auto h = (0.05);
//     auto rect_w = sw * w;
//     auto rect_h = sh * h;
//     auto offs_x = (sw / 2) - (rect_w * tile_amt_x / 2);
//     auto offs_y = (sh / 2) - (rect_h * tile_amt_y / 2);
//     auto reg = ge->registry();
//     // TODO this logic can be remade
//     for (auto [ent, pos, size, selectable, hoverable, node] :
//          reg->get().view<Component::Pos, Component::Size, Interaction::Selectable,
//          Interaction::Hoverable, Component::Combat::Node>()
//              .each()) {
//         if (event.get_button() == MouseButtonEvent::MouseButton::LeftMouseButton) {
//             if (hoverable.is_hovered) {
//                 auto x_normalized = ((pos.x - offs_x) / rect_w);
//                 auto y_normalized = ((pos.y - offs_y) / rect_h);
//                 auto index_pos = x_normalized + (tile_amt_x * y_normalized);
//                 if (selectable.is_selected) {
//                     selectable.is_selected = false;
//                     node.is_obstacle = false;
//                 } else {
//                     selectable.is_selected = true;
//                     node.is_obstacle = true;
//                 }
//                 return true;
//             }
//         }
//         if (event.get_button() == MouseButtonEvent::MouseButton::RightMouseButton) {
//             if (hoverable.is_hovered) {
//                 auto x_normalized = ((pos.x - offs_x) / rect_w);
//                 auto y_normalized = ((pos.y - offs_y) / rect_h);
//                 auto index_pos = x_normalized + (tile_amt_x * y_normalized);
//                 node.is_obstacle = false;
//                 reg->remove_all_instances_of_tag<Component::Combat::_Node_Start>();
//                 reg->add_tag<Component::Combat::_Node_Start>(ent);
//                 return true;
//             }
//         }
//     }

//     return false;
//     }
}; // namespace PlayingState
