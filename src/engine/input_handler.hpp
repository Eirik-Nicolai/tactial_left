#pragma once
#include "logger.hpp"
#include "input.hpp"

class TacticalGame;
class InputHandler {
  public:
    InputHandler();

    void unregister_input(INPUT_TYPE type);

    void register_input(INPUT_TYPE type, std::shared_ptr<Input> inp);

    std::shared_ptr<Input> get_input(TacticalGame* ge);

  private:
    std::array<std::shared_ptr<Input>, 40> m_registered_inputs;
    std::shared_ptr<NULLInput> NONE;
};
