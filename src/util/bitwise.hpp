#ifndef GBEMU_UTIL_BITWISE_HPP
#define GBEMU_UTIL_BITWISE_HPP
#include "../definitions.hpp"
#include "log.h"

namespace bitwise {

constexpr auto compose_bits(u8 const high, u8 const low) -> u8
{
    return static_cast<u8>(high << 1 | low);
}

constexpr auto compose_nibbles(u8 const high, u8 const low) -> u8
{
    return static_cast<u8>((high << 4) | low);
}

constexpr auto compose_bytes(u8 const high, u8 const low) -> u16
{
    return static_cast<u16>((high << 8) | low);
}

constexpr auto check_bit(u8 const value, u8 const bit) -> bool
{
    return (value & (1 << bit)) != 0;
}

constexpr auto bit_value(u8 const value, u8 const bit) -> u8
{
    return (value >> bit) & 1;
}

constexpr auto set_bit(u8 const value, u8 const bit) -> u8
{
    auto const value_set = value | (1 << bit);
    return static_cast<u8>(value_set);
}

constexpr auto clear_bit(u8 const value, u8 const bit) -> u8
{
    auto const value_cleared = value & ~(1 << bit);
    return static_cast<u8>(value_cleared);
}

constexpr auto set_bit_to(u8 const value, u8 const bit, bool bit_on) -> u8
{
    return bit_on ? set_bit(value, bit) : clear_bit(value, bit);
}

}  // namespace bitwise
#endif  // GBEMU_UTIL_BITWISE_HPP
