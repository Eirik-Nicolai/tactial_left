#pragma once
#include "game.hpp"
#include "utils/debugging.hpp"
constexpr auto MAX_SYSTEM_AMOUNT = 20;

class System {
  GET_BASE(System)

  public:
    virtual void execute(TacticalGame* ge) = 0;
};

class SystemManager{
  GET_BASE(SystemManager)
  public:
    virtual void dispatch(TacticalGame* ge) = 0;
    virtual void add(std::unique_ptr<System> system) = 0;
  protected:
    unsigned m_system_amount;
    std::array<std::unique_ptr<System>, MAX_SYSTEM_AMOUNT> m_systems;
};
