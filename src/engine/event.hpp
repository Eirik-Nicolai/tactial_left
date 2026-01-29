#pragma once

#include "logger.hpp"
#include "entt/entt.hpp"
#include <memory_resource>
#include <string>

class Event {
  public:
    Event(std::string_view type_) : name(type_) {
      type = std::hash<std::string_view>{}(type_);
    }

    virtual ~Event() = default;
    virtual std::string_view get_name() const { return name; }
    virtual std::uint32_t get_type() const { return type; };

    bool isHandled { false };

  private:
    std::string_view name;
    uint32_t type;
};

class Observer
{
  friend class Subject;

  public:
    Observer(): next_(nullptr) {}
    virtual ~Observer() {}
    virtual void on_notify(const entt::registry& reg, const Event& event) = 0;

  private:
    Observer* next_;
};


class Subject
{
  public:
    Subject(): head_(nullptr) {}

    inline void add_observer(Observer* observer)
    {
      observer->next_ = head_;
      head_ = observer;
    }

    void removeObserver(Event &event, Observer* observer)
    {
        if (head_ == observer)
        {
          head_ = observer->next_;
          observer->next_ = NULL;
          return;
        }

        Observer* current = head_;
        while (current != NULL)
        {
          if (current->next_ == observer)
          {
            current->next_ = observer->next_;
            observer->next_ = NULL;
            return;
          }

          current = current->next_;
        }
    }

    void notify(const entt::registry& reg, const Event& event)
    {
        Observer* observer = head_;
        while (observer != NULL)
        {
          observer->on_notify(reg, event);
          observer = observer->next_;
        }
    }

  private:
    Observer* head_;
};
