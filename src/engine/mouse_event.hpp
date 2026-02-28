#pragma once

#include <string>
#include "magic_enum.hpp"
#include "event.hpp"

namespace Engine
{
//
// Mouse Events
//
class MouseEvent : public Event
{
  public:
    MouseEvent(int x, int y) : m_x(x), m_y(y) {}

    inline int GetX() const { return m_x; }
    inline int GetY() const { return m_y; }

  private:
    int m_x, m_y;
};

class MouseMovedEvent : public MouseEvent
{
  public:
    MouseMovedEvent(int x, int y) : MouseEvent(x, y) {}

    // std::string to_string() const override {
    //     Debug("MouseMovedEvent: {}, {}", m_x, m_y);

    // }

    EVENT_CLASS_TYPE(MouseMoved)
};

// class MouseScrolledEvent : public MouseEvent {
// public:
//   MouseScrolledEvent(int xOffset, int yOffset)
//       : m_XOffset(xOffset), m_YOffset(yOffset) {}

//   inline int GetXOffset() const { return m_XOffset; }
//   inline int GetYOffset() const { return m_YOffset; }

//   std::string to_string() const override {
//     Debug("MouseScrolledEvent: {}, {}", m_XOffset, m_YOffset);
//   }

//   EVENT_CLASS_TYPE(MouseScrolled)
// private:
//   int m_XOffset, m_YOffset;
// };

class MouseButtonEvent : public MouseEvent
{

  public:
    enum class MouseButton : uint8_t {
        LeftMouseButton = 0,
        RightMouseButton,
        MiddleMouseButton,
    };

    inline MouseButton get_button() const { return m_button; }

  protected:
    MouseButtonEvent(int x, int y, int button)
        : MouseEvent(x, y), m_button(static_cast<MouseButton>(button))
    {
    }
    MouseButtonEvent(int x, int y, MouseButton button)
        : MouseEvent(x, y), m_button(button)
    {
    }

    MouseButton m_button;
};

class MouseButtonPressedEvent : public MouseButtonEvent
{
  public:
    MouseButtonPressedEvent(int x, int y, int button)
        : MouseButtonEvent(x, y, button)
    {
    }

    std::string to_string() const
    {
        std::stringstream ss;
        ss << "MouseButtonPressedEvent(" << magic_enum::enum_name(get_button()) << ")";
        return ss.str();
    }
    // std::string to_string() const override {
    //   Debug("MouseButtonPressedEvent: {}", static_cast<uint8_t>(m_button));
    // }
    EVENT_CLASS_TYPE(MouseButtonPressed)
};

class MouseButtonReleasedEvent : public MouseButtonEvent
{
  public:
    MouseButtonReleasedEvent(int x, int y, int button)
        : MouseButtonEvent(x, y, button)
    {
    }

    std::string to_string() const
    {
        std::stringstream ss;
        ss << "MouseButtonReleasedEvent(" << magic_enum::enum_name(get_button()) << ")";
        return ss.str();
    }
    EVENT_CLASS_TYPE(MouseButtonReleased)
};

}; // namespace Engine
