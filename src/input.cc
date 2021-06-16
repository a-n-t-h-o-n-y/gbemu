#include "input.h"

#include "util/bitwise.h"

void Input::button_pressed(GbButton button) {
    set_button(button, true);
}

void Input::button_released(GbButton button) {
    set_button(button, false);
}

void Input::set_button(GbButton button, bool set) {
    switch(button) {
        case GbButton::Up: up = set; break;
        case GbButton::Down: down = set; break;
        case GbButton::Left: left = set; break;
        case GbButton::Right: right = set; break;
        case GbButton::A: a = set; break;
        case GbButton::B: b = set; break;
        case GbButton::Select: select = set; break;
        case GbButton::Start: start = set; break;
    }
}

void Input::write(u8 set) {
    using bitwise::check_bit;

    direction_switch = !check_bit(set, 4);
    button_switch = !check_bit(set, 5);
}

u8 Input::get_input() const {
    using bitwise::set_bit_to;

    u8 buttons = 0b1111;

    if (direction_switch) {
        buttons = set_bit_to(buttons, 0, !right);
        buttons = set_bit_to(buttons, 1, !left);
        buttons = set_bit_to(buttons, 2, !up);
        buttons = set_bit_to(buttons, 3, !down);
    }

    if (button_switch) {
        buttons = set_bit_to(buttons, 0, !a);
        buttons = set_bit_to(buttons, 1, !b);
        buttons = set_bit_to(buttons, 2, !select);
        buttons = set_bit_to(buttons, 3, !start);
    }

    buttons = set_bit_to(buttons, 4, !direction_switch);
    buttons = set_bit_to(buttons, 5, !button_switch);

    return buttons;
}
