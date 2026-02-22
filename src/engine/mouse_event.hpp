#pragma once

#include <string>
#include "magic_enum.hpp"
#include "event.hpp"

//
// Mouse Events
//
class MouseMovedEvent : public Event
{
  public:
    MouseMovedEvent(double x, double y) : m_x(x), m_y(y) {}

    inline double GetX() const { return m_x; }
    inline double GetY() const { return m_y; }

    // std::string to_string() const override {
    //     Debug("MouseMovedEvent: {}, {}", m_x, m_y);

    // }

    EVENT_CLASS_TYPE(MouseMoved)
  private:
    int m_x, m_y;
};

// class MouseScrolledEvent : public Event {
// public:
//   MouseScrolledEvent(double xOffset, double yOffset)
//       : m_XOffset(xOffset), m_YOffset(yOffset) {}

//   inline double GetXOffset() const { return m_XOffset; }
//   inline double GetYOffset() const { return m_YOffset; }

//   std::string to_string() const override {
//     Debug("MouseScrolledEvent: {}, {}", m_XOffset, m_YOffset);
//   }

//   EVENT_CLASS_TYPE(MouseScrolled)
// private:
//   double m_XOffset, m_YOffset;
// };

class MouseButtonEvent : public Event
{

  public:
    enum class MouseButton : uint8_t {
        LeftMouseButton = 0,
        RightMouseButton,
        MiddleMouseButton,
    };

    inline MouseButton get_button() const { return m_button; }

  protected:
    MouseButtonEvent(int button) : m_button(static_cast<MouseButton>(button)) {}
    MouseButtonEvent(MouseButton button) : m_button(button) {}

    MouseButton m_button;
};

class MouseButtonPressedEvent : public MouseButtonEvent
{
  public:
    MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}

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
    MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}

    std::string to_string() const
    {
        std::stringstream ss;
        ss << "MouseButtonReleasedEvent(" << magic_enum::enum_name(get_button()) << ")";
        return ss.str();
    }
    EVENT_CLASS_TYPE(MouseButtonReleased)
};
