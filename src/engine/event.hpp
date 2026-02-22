#pragma once

#include "entt/entt.hpp"
#include <string>

enum class EventType {
    NoneEvent = 0,
    // WindowClose, // to be implemented
    // WindowResize,// to be implemented
    KeyPressed,
    KeyReleased,
    MouseButtonPressed,
    MouseButtonReleased,
    MouseMoved,
    MouseScrolled,

    // Audio
    // Music
};

#define EVENT_CLASS_TYPE(type)                                                           \
  public:                                                                                \
    static EventType get_static_type() { return EventType::type; }                       \
    virtual EventType get_type() const override { return get_static_type(); }            \
    std::string_view get_name() const override  { return #type; }

class Event
{
  public:
    bool consumed = false;
    virtual ~Event() {}
    virtual EventType get_type() const = 0;
    virtual std::string_view get_name() const = 0;
  private:
};

class NoneEvent : public Event
{
    NoneEvent() = default;
    EVENT_CLASS_TYPE(NoneEvent)
};

class TacticalGame;
class EventDispatcher
{
    template <typename T> using EventFn = std::function<bool(TacticalGame *ge, T &)>;

  public:
    EventDispatcher(TacticalGame *ge, Event &event) : m_ge(ge), m_event(event) {}

    template <typename T> bool Dispatch(EventFn<T> func)
    {
        // let the different gamestates dispatch a method to handle the event
        // that they want to use
        if (m_event.get_type() == T::get_static_type() && !m_event.consumed) {
            m_event.consumed = func(m_ge, *(T *)&m_event);
            return true;
        }
        return false;
    }

  private:
    TacticalGame *m_ge;
    Event &m_event;
};
