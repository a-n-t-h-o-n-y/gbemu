#include "serial.hpp"

#include "util/bitwise.hpp"
#include "util/log.h"

#include <cstdio>

auto Serial::read() const -> u8 { return data; }

void Serial::write(const u8 byte) { data = byte; }

void Serial::write_control(const u8 byte)
{
    if (bitwise::check_bit(byte, 7) && options.print_serial) {
        std::printf("%c", data);
        std::fflush(stdout);
    }
}
