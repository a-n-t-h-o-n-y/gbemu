#ifndef GBEMU_REGISTER_HPP
#define GBEMU_REGISTER_HPP
#include "definitions.hpp"
#include "util/bitwise.hpp"

class FlagRegister {
   public:
    constexpr FlagRegister(u8 value = 0) : val_{value} {}

   public:
    constexpr void set(u8 new_value) { val_ = new_value; }

    constexpr void reset() { val_ = 0; }

    [[nodiscard]] constexpr auto value() const -> u8 { return val_; }

    [[nodiscard]] constexpr auto check_bit(u8 bit) const -> bool
    {
        return bitwise::check_bit(val_, bit);
    }

    constexpr void set_bit_to(u8 bit, bool set)
    {
        val_ = bitwise::set_bit_to(val_, bit, set);
    }

    constexpr void increment() { ++val_; }

    constexpr void decrement() { --val_; }

    constexpr void set_flag_zero(bool set) { this->set_bit_to(7, set); }

    constexpr void set_flag_subtract(bool set) { this->set_bit_to(6, set); }

    constexpr void set_flag_half_carry(bool set) { this->set_bit_to(5, set); }

    constexpr void set_flag_carry(bool set) { this->set_bit_to(4, set); }

    [[nodiscard]] constexpr auto flag_zero() const -> bool
    {
        return this->check_bit(7);
    }

    [[nodiscard]] constexpr auto flag_subtract() const -> bool
    {
        return this->check_bit(6);
    }

    [[nodiscard]] constexpr auto flag_half_carry() const -> bool
    {
        return this->check_bit(5);
    }

    [[nodiscard]] constexpr auto flag_carry() const -> bool
    {
        return this->check_bit(4);
    }

    constexpr auto flag_zero_value() const -> u8
    {
        return static_cast<u8>(this->flag_zero() ? 1 : 0);
    }

    constexpr auto flag_subtract_value() const -> u8
    {
        return static_cast<u8>(this->flag_subtract() ? 1 : 0);
    }

    constexpr auto flag_half_carry_value() const -> u8
    {
        return static_cast<u8>(this->flag_half_carry() ? 1 : 0);
    }

    constexpr auto flag_carry_value() const -> u8
    {
        return static_cast<u8>(flag_carry() ? 1 : 0);
    }

   private:
    u8 val_;
};

[[nodiscard]] constexpr auto operator==(FlagRegister lhs, FlagRegister rhs)
    -> bool
{
    return lhs.value() == rhs.value();
}

class WordRegister {
   public:
    constexpr explicit WordRegister(u16 val = 0) : val_{val} {}

   public:
    constexpr void set(u16 new_value) { val_ = new_value; }

    constexpr auto value() const -> u16 { return val_; }

    constexpr auto low() const -> u8 { return static_cast<u8>(val_); }

    constexpr auto high() const -> u8 { return static_cast<u8>((val_) >> 8); }

    constexpr void increment() { ++val_; }

    constexpr void decrement() { --val_; }

   private:
    u16 val_;
};

class RegisterPair {
   public:
    constexpr RegisterPair(FlagRegister& high, FlagRegister& low)
        : high_byte_(high), low_byte_(low)
    {}

   public:
    constexpr void set(u16 word)
    {
        low_byte_.set(static_cast<u8>(word));
        high_byte_.set(static_cast<u8>((word) >> 8));
    }

    constexpr auto value() const -> u16
    {
        return bitwise::compose_bytes(high_byte_.value(), low_byte_.value());
    }

    constexpr auto low() const -> u8 { return low_byte_.value(); }

    constexpr auto high() const -> u8 { return high_byte_.value(); }

    constexpr void increment() { this->set(this->value() + 1); }

    constexpr void decrement() { this->set(this->value() - 1); }

   private:
    FlagRegister& high_byte_;
    FlagRegister& low_byte_;
};

#endif  // GBEMU_REGISTER_HPP
