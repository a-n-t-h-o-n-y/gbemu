#pragma once

#include "../definitions.h"
#include "log.h"

namespace bitwise {

constexpr u8 compose_bits(const u8 high, const u8 low)
{
    return static_cast<u8>(high << 1 | low);
}

constexpr u8 compose_nibbles(const u8 high, const u8 low)
{
    return static_cast<u8>((high << 4) | low);
}

constexpr u16 compose_bytes(const u8 high, const u8 low)
{
    return static_cast<u16>((high << 8) | low);
}

constexpr bool check_bit(const u8 value, const u8 bit)
{
    return (value & (1 << bit)) != 0;
}

constexpr u8 bit_value(const u8 value, const u8 bit)
{
    return (value >> bit) & 1;
}

constexpr u8 set_bit(const u8 value, const u8 bit)
{
    auto value_set = value | (1 << bit);
    return static_cast<u8>(value_set);
}

constexpr u8 clear_bit(const u8 value, const u8 bit)
{
    auto value_cleared = value & ~(1 << bit);
    return static_cast<u8>(value_cleared);
}

constexpr u8 set_bit_to(const u8 value, const u8 bit, bool bit_on)
{
    return bit_on ? set_bit(value, bit) : clear_bit(value, bit);
}

}  // namespace bitwise
