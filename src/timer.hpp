#ifndef GBEMU_TIMER_HPP
#define GBEMU_TIMER_HPP
#include "definitions.hpp"
#include "register.hpp"

class Timer {
   public:
    void tick(uint cycles);

    auto get_divider() const -> u8;
    auto get_timer() const -> u8;
    auto get_timer_modulo() const -> u8;
    auto get_timer_control() const -> u8;

    void reset_divider();
    void set_timer_modulo(u8 value);
    void set_timer_control(u8 value);

   private:
    FlagRegister divider;
    FlagRegister timer_counter;

    FlagRegister timer_modulo;
    FlagRegister timer_control;
};

#endif  // GBEMU_TIMER_HPP
