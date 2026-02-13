#include "input_handler.hpp"

InputHandler::InputHandler()
{
    NONE = std::make_shared<NULLInput>();

    m_registered_inputs = std::array<std::shared_ptr<Input>, 40>();
    m_registered_inputs[INPUT_TYPE::key_P_PRESSED] = NONE;
    m_registered_inputs[INPUT_TYPE::key_P_RELEASED] = NONE;
    m_registered_inputs[INPUT_TYPE::left_mouse_PRESSED] = NONE;
    m_registered_inputs[INPUT_TYPE::left_mouse_HELD] = NONE;
    m_registered_inputs[INPUT_TYPE::left_mouse_RELEASED] = NONE;
    m_registered_inputs[INPUT_TYPE::right_mouse_PRESSED] = NONE;
    m_registered_inputs[INPUT_TYPE::right_mouse_HELD] = NONE;
    m_registered_inputs[INPUT_TYPE::right_mouse_RELEASED] = NONE;
    m_registered_inputs[INPUT_TYPE::middle_mouse_PRESSED] = NONE;
    m_registered_inputs[INPUT_TYPE::middle_mouse_HELD] = NONE;
    m_registered_inputs[INPUT_TYPE::middle_mouse_RELEASED] = NONE;
    m_registered_inputs[INPUT_TYPE::middle_mouse_SCROLLUP] = NONE;
    m_registered_inputs[INPUT_TYPE::middle_mouse_SCROLLDOWN] = NONE;
}

void InputHandler::unregister_input(INPUT_TYPE type)
{
    Error("unregister_input not implemented");
    // TODO fix
    // m_registered_inputs[type] = NONE;
}

void InputHandler::register_input(INPUT_TYPE type, std::shared_ptr<Input> inp)
{
    if (!inp) {
        Error("Cannot register a nullptr as input");
        m_registered_inputs[type] = NONE;
        return;
    }
    Debug("Registering input {}", inp->get_name());
    m_registered_inputs[type] = std::move(inp);
}

std::shared_ptr<Input> InputHandler::get_input(TacticalGame *ge)
{
    if (ge->GetKey(olc::Key::P).bPressed)
        return m_registered_inputs[INPUT_TYPE::key_P_PRESSED];
    if (ge->GetKey(olc::Key::P).bReleased)
        return m_registered_inputs[INPUT_TYPE::key_P_RELEASED];
    if (ge->GetMouse(MOUSE_LBUTTON).bPressed)
        return m_registered_inputs[INPUT_TYPE::left_mouse_PRESSED];
    if (ge->GetMouse(MOUSE_LBUTTON).bReleased)
        return m_registered_inputs[INPUT_TYPE::left_mouse_RELEASED];
    if (ge->GetMouse(MOUSE_LBUTTON).bHeld)
        return m_registered_inputs[INPUT_TYPE::left_mouse_HELD];
    if (ge->GetMouse(MOUSE_RBUTTON).bPressed)
        return m_registered_inputs[INPUT_TYPE::right_mouse_PRESSED];
    if (ge->GetMouse(MOUSE_RBUTTON).bReleased)
        return m_registered_inputs[INPUT_TYPE::right_mouse_RELEASED];
    if (ge->GetMouse(MOUSE_RBUTTON).bHeld)
        return m_registered_inputs[INPUT_TYPE::right_mouse_HELD];
    if (ge->GetMouse(MOUSE_MBUTTON).bPressed)
        return m_registered_inputs[INPUT_TYPE::middle_mouse_PRESSED];
    if (ge->GetMouse(MOUSE_MBUTTON).bReleased)
        return m_registered_inputs[INPUT_TYPE::middle_mouse_RELEASED];
    if (ge->GetMouse(MOUSE_MBUTTON).bHeld)
        return m_registered_inputs[INPUT_TYPE::middle_mouse_HELD];
    if (ge->GetMouseWheel() < 0)
        return m_registered_inputs[INPUT_TYPE::middle_mouse_SCROLLDOWN];
    if (ge->GetMouseWheel() > 0)
        return m_registered_inputs[INPUT_TYPE::middle_mouse_SCROLLUP];

    return NONE;
}
