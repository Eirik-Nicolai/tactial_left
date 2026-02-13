#pragma once
#include "game.hpp"
#include "logger.hpp"

#define MOUSE_LBUTTON 0
#define MOUSE_RBUTTON 1
#define MOUSE_MBUTTON 2

enum INPUT_TYPE {
key_P_PRESSED,
key_P_RELEASED,

left_mouse_PRESSED,
left_mouse_HELD,
left_mouse_RELEASED,

right_mouse_PRESSED,
right_mouse_HELD,
right_mouse_RELEASED,

middle_mouse_PRESSED,
middle_mouse_HELD,
middle_mouse_RELEASED,
middle_mouse_SCROLLUP,
middle_mouse_SCROLLDOWN
};


class TacticalGame;
class Input{
  public:
    virtual std::string get_name() const { return "InputBase"; };
    virtual void execute(TacticalGame*) = 0;
};

class NULLInput : public Input{
  public:
    virtual std::string get_name() const { return "NONEInput"; };
    inline void execute(TacticalGame* ge) final {
      std::ignore = ge;
      // do nothing
    };
};

class InputDebugWrapper : public Input {
  private:
    std::shared_ptr<Input> input;
  public:
    InputDebugWrapper(std::shared_ptr<Input> wrapped = std::make_shared<NULLInput>()) : input(std::move(wrapped)) {}
    virtual std::string get_name() const { return "InputLogWrapper"; };
    inline void execute(TacticalGame* ge) final {
      //Trace("Executing input: {}", input->get_name());
      input->execute(ge);
    };

    std::shared_ptr<Input> get_underlying() { return input; }
};
