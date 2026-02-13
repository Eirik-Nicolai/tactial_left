#pragma once

#include "event.hpp"
#include "olc/olcPixelGameEngine.h"
//
// Key Events
//

class KeyEvent : public Event {
public:
  inline olc::Key get_key() const { return m_key; }

protected:
  KeyEvent(int keycode) : m_key(static_cast<olc::Key>(keycode)) {}
  KeyEvent(olc::Key keycode) : m_key(keycode) {}

  olc::Key m_key;
};

class KeyPressedEvent : public KeyEvent {
public:
  KeyPressedEvent(int keycode, bool isRepeat)
      : KeyEvent(keycode), m_is_repeating(isRepeat) {}

  inline bool is_repeating() const { return m_is_repeating; }

  // std::string to_string() const override {
  //   Debug("KeyPressedEvent: {} (repeat={})", 1, m_is_repeating);
  // }

  EVENT_CLASS_TYPE(KeyPressed)
private:
  bool m_is_repeating;
};

class KeyReleasedEvent : public KeyEvent {
public:
  KeyReleasedEvent(int keycode) : KeyEvent(keycode) {}

  // std::string to_string() const override {
  //   Debug("KeyReleasedEvent: {}", 1);
  // }

  EVENT_CLASS_TYPE(KeyReleased)
};
