#pragma once
#include "game.hpp"

constexpr auto MAX_SYSTEM_AMOUNT = 20;

class System {
  public:
    virtual std::string_view get_name() { return "System"; }
    virtual void execute(TacticalGame* ge) = 0;
};

class SystemManager{
  public:
    virtual std::string_view get_name() { return "SystemManager"; }
    virtual void dispatch(TacticalGame* ge) = 0;
    virtual void add(std::unique_ptr<System> system) = 0;
  protected:
    unsigned m_system_amount;
    std::array<std::unique_ptr<System>, MAX_SYSTEM_AMOUNT> m_systems;
};
