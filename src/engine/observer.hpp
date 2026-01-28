#pragma once
#include "logger.hpp"
#include "entt/entt.hpp"

constexpr auto MAX_OBSERVERS = 10;

class Event
{
    public:
        virtual std::string get_info() = 0;
};

class Observer
{
    public:
        virtual ~Observer() {}
        virtual void onNotify(const entt::registry& reg, const Event& event) = 0;
};


class Subject
{
    public:
        void addObserver(Observer* observer)
        {
            if(m_numObservers == MAX_OBSERVERS) {
                Error("Unable to add observer, max number {} reached", MAX_OBSERVERS);
            }
            m_observers[m_numObservers] = observer;
            ++m_numObservers;
        }

        void removeObserver(Observer* observer)
        {
            // TODO rewrite this sloppy mess
            // just for testing
            std::array<Observer*, MAX_OBSERVERS> m_nObservers;
            for (int i = 0; i < m_numObservers; i++)
            {
                if(m_observers[i] != observer) {
                    m_nObservers[i] = m_observers[i];
                }
            }
            m_observers = m_nObservers;
        }

        void notify(const entt::registry& reg, Event event)
        {
            for (int i = 0; i < m_numObservers; i++)
            {
                m_observers[i]->onNotify(reg, event);
            }
        }
    private:
        std::array<Observer*, MAX_OBSERVERS> m_observers;
        int m_numObservers;
};
