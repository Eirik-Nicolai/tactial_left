#include "utils/geometry.hpp"
#define OLC_PGEX_FUI
#include "debug_layer.hpp"

// #define PGEX_UI
// #include "olc/olcPGEX_UI.h"

DebugLayer::DebugLayer(TacticalGame *ge)
{
    m_ui_manager.create_window("groupbox", "TEST", {0, 0}, {300, 400});
    m_ui_manager.find_window("groupbox")->disable_close(true);
    m_ui_manager.add_dropdown("groupbox", "resolution", "Resolution: ", {80, 10},
                              {100, 20});
    m_ui_manager.find_element("resolution")->add_item("1920x1080");
    m_ui_manager.find_element("resolution")->add_item("1280x720");
    m_ui_manager.find_element("resolution")->add_item("800x600");
    m_ui_manager.find_element("resolution")->add_item("640x360");
    m_ui_manager.find_element("resolution")->set_default_item(0);
    m_ui_manager.find_element("resolution")->set_animation_speed(1000);
    m_ui_manager.find_element("resolution")->set_max_display_items(4);

    m_ui_manager.add_dropdown("groupbox", "quality", "Quality: ", {80, 40}, {100, 20});
    m_ui_manager.find_element("quality")->add_item("High", {1.f, 1.f});
    m_ui_manager.find_element("quality")->add_item("Medium");
    m_ui_manager.find_element("quality")->add_item("Low");
    m_ui_manager.find_element("quality")->set_default_item(0);
    m_ui_manager.find_element("quality")->set_animation_speed(1000);
    m_ui_manager.find_element("quality")->set_max_display_items(5);

    m_ui_manager.add_checkbox("groupbox", "fullscreen", "Fullscreen: ", {80, 70},
                              {20, 20});

    m_ui_manager.add_inputfield("groupbox", "custom_title", "Custom title: ", {80, 100},
                                {100, 20});
    m_ui_manager.find_element("custom_title")->inputfield_scale({1.25f, 1.25f});

    m_ui_manager.add_button(
        "groupbox", "launch", "Launch game", {10, 130}, {180, 20}, [&] {
            auto x = m_ui_manager.find_element("slider")->get_slider_value<int>();
            auto y = m_ui_manager.find_element("slider1")->get_slider_value<float>();
            auto k = m_ui_manager.find_element("fullscreen")->get_checkbox_state();

            std::cout << x << " - " << y << " - " << k << "\n";
        });
    m_ui_manager.find_element("resolution")
        ->set_colors({olc::Pixel(120, 120, 120), olc::Pixel(100, 100, 100),
                      olc::Pixel(80, 80, 80)});

    m_ui_manager.add_int_slider("groupbox", "slider", "Testing: ", {80, 160}, {100, 10},
                                {-5, 5});
    m_ui_manager.find_element("slider")->set_slider_value(
        2000); // the set_slider_value function will clamp this value
               // automatically
    m_ui_manager.add_float_slider("groupbox", "slider1", "Testing 2: ", {80, 190},
                                  {100, 10}, {-155, 372});
    m_ui_manager.find_element("slider1")->set_slider_value(
        100000.f); // .f will indicate that this is a float value (this function
                   // is templated to avoid implicit casting)
}

DebugLayer::~DebugLayer() {}

void DebugLayer::update(TacticalGame *ge)
{

    m_ui_manager.run();

    // if()
}

void DebugLayer::draw(TacticalGame *) {}

void DebugLayer::on_event(TacticalGame *ge, Event &e)
{
    Error("This is an error");
    EventDispatcher dispatcher(ge, e);
    dispatcher.Dispatch<MouseButtonReleasedEvent>(
        [this](TacticalGame *ge, MouseButtonReleasedEvent &e) {
            return mouse_button_released(ge, e);
        });

    dispatcher.Dispatch<MouseButtonPressedEvent>(
        [this](TacticalGame *ge, MouseButtonPressedEvent &e) {
            return mouse_button_pressed(ge, e);
        });
}
// bool DebugLayer::key_released(TacticalGame *, KeyReleasedEvent &event) {

// }
// bool DebugLayer::key_pressed(TacticalGame *, KeyPressedEvent &event) {

// }

bool DebugLayer::mouse_button_released(TacticalGame *ge, MouseButtonReleasedEvent &event)
{
    auto window_pos = m_ui_manager.find_window("groupbox")->get_position();
    auto window_size = m_ui_manager.find_window("groupbox")->get_window_space();
    auto mouse = ge->GetMousePos();
    if (is_point_inside_rect(window_pos.x, window_pos.y, window_size.x, window_size.y,
                             mouse.x, mouse.y)) {
        Info("Released debug");
        return false;
    }
    return false;
}
bool DebugLayer::mouse_button_pressed(TacticalGame *ge, MouseButtonPressedEvent &event)
{
    auto window_pos = m_ui_manager.find_window("groupbox")->get_position();
    auto window_size = m_ui_manager.find_window("groupbox")->get_window_space();
    auto mouse = ge->GetMousePos();
    if (is_point_inside_rect(window_pos.x, window_pos.y, window_size.x, window_size.y,
                             mouse.x, mouse.y)) {
        Info("pressed debug");
        return true;
    }
    return false;
}
