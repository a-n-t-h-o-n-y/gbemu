#ifndef GBEMU_INPUT_HPP
#define GBEMU_INPUT_HPP
#include "definitions.hpp"

enum class GbButton { Up, Down, Left, Right, A, B, Select, Start };

class Input {
   public:
    void button_pressed(GbButton button);

    void button_released(GbButton button);

    void write(u8 set);

    auto get_input() const -> u8;

   private:
    void set_button(GbButton button, bool set);

   private:
    bool up     = false;
    bool down   = false;
    bool left   = false;
    bool right  = false;
    bool a      = false;
    bool b      = false;
    bool select = false;
    bool start  = false;

    bool button_switch    = false;
    bool direction_switch = false;
};

#endif  // GBEMU_INPUT_HPP
