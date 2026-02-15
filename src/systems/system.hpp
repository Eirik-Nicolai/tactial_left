#pragma once
#include "game.hpp"
#include "utils/debugging.hpp"

class TacticalGame;
constexpr auto MAX_SYSTEM_AMOUNT = 20;
class System
{
    GET_BASE(System)

  public:
    virtual void execute(TacticalGame *ge) = 0;
};

class SystemManager
{
  GET_BASE(SystemManager)
  public:
    SystemManager(const char *name)
    {
        m_systems = std::array<std::unique_ptr<System>, MAX_SYSTEM_AMOUNT>();
        m_system_amount = 0;
        m_name = name;
    }
    inline void dispatch(TacticalGame *ge)
    {
        for (int i = 0; i < m_system_amount; ++i) {
            m_systems[i]->execute(ge);
        }
    }
    inline void add(std::unique_ptr<System> system)
    {
        Debug("Adding to list of {}, size: {}", get_name(), m_system_amount);
        if (m_system_amount < m_systems.max_size()) {
            m_systems[m_system_amount] = std::move(system);
            m_system_amount++;
        } else {
            Error("System manager {} is full !", get_name());
        }
    }

  protected:
    std::string m_name;
    unsigned m_system_amount;
    std::array<std::unique_ptr<System>, MAX_SYSTEM_AMOUNT> m_systems;
};

// TODO move all this to arent class init since we don't change it between
// types of managers
// or make templated ? unsure what is better

// inline void SystemManager::add(std::unique_ptr<System> system) {
//     Debug("Adding to list of systems, size: {}", m_system_amount);
//     if(m_system_amount < m_systems.max_size()) {
//         m_systems[m_system_amount] = std::move(system);
//         m_system_amount++;
//     } else {
//         Error("System manager {} is full !", get_name());
//     }
// }

// inline void SystemManager::dispatch(TacticalGame* ge) {
//     for(int i = 0; i < m_system_amount; ++i) {
//         m_systems[i]->execute(ge);
//     }
// }
