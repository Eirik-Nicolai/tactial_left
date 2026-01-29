#pragma once
#include "logger.hpp"
#include "input.hpp"

class TacticalGame;
class InputHandler {
  public:
    InputHandler();

    void unregister_input(INPUT_TYPE type);

    void register_input(INPUT_TYPE type, Input* inp);

    Input* get_input(TacticalGame* ge);

  private:
    //TODO could make into a C array for speed ?
    std::map<INPUT_TYPE, Input*> m_registered_inputs;
    NULLInput* NONE;
};
