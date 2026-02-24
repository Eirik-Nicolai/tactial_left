#pragma once
#include "logger.hpp"
#include "input.hpp"
#include "game_input_event.hpp"

#include "utils/debugging.hpp"
class TacticalGame;
class InputHandler
{
  public:
    GET_BASE(InputHandler)
    InputHandler();
    
    void unregister_input(std::string&);
    void unregister_input(INPUT_TYPE type);
    using InputFunction = std::function<void(TacticalGame*, Engine::GameEvent&)>;
    // void register_input(std::string type, InputFunction i) {
    //     m_inputs[type] = std::move(i);
    // }
    void register_input(INPUT_TYPE type, std::shared_ptr<Input> inp);

    std::shared_ptr<Input> get_input(TacticalGame *ge);

  private:
    // std::map<std::string_view,InputFunction> m_inputs;
    std::array<std::shared_ptr<Input>, 40> m_registered_inputs;
    std::shared_ptr<NULLInput> NONE;
};
