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
    MouseEvent(int sx, int sy, int wx, int wy) : m_sx(sx), m_sy(sy), m_wx(wx), m_wy(wy) {}

    std::pair<int,int> get_screen_pos() const { return std::make_pair(m_sx, m_sy); }
    std::pair<int,int> get_world_pos() const { return std::make_pair(m_wx, m_wy); }

  private:
    int m_sx, m_sy; // screen space pos
    int m_wx, m_wy; // world space pos
};

class MouseMovedEvent : public MouseEvent
{
  public:
    MouseMovedEvent(int sx, int sy, int wx, int wy) : MouseEvent(sx, sy, wx, wy) {}

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
    MouseButtonEvent(int sx, int sy, int wx, int wy, int button)
        : MouseEvent(sx, sy, wx, wy), m_button(static_cast<MouseButton>(button))
    {
    }
    MouseButtonEvent(int sx, int sy, int wx, int wy, MouseButton button)
        : MouseEvent(sx, sy, wx, wy), m_button(button)
    {
    }

    MouseButton m_button;
};

class MouseButtonPressedEvent : public MouseButtonEvent
{
  public:
    MouseButtonPressedEvent(int sx, int sy, int wx, int wy, int button)
        : MouseButtonEvent(sx, sy, wx, wy, button)
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
    MouseButtonReleasedEvent(int sx, int sy, int wx, int wy, int button)
        : MouseButtonEvent(sx, sy, wx, wy, button)
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
