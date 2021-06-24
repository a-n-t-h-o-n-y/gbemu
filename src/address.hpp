#ifndef GBEMU_ADDRESS_HPP
#define GBEMU_ADDRESS_HPP
#include "definitions.hpp"
#include "register.hpp"

class Address {
   public:
    constexpr Address(u16 location) : addr_{location} {}

   public:
    [[nodiscard]] constexpr auto value() const -> u16 { return addr_; }

   private:
    u16 addr_;
};

[[nodiscard]] constexpr auto to_address(RegisterPair from) -> Address
{
    return {from.value()};
}

[[nodiscard]] constexpr auto to_address(WordRegister from) -> Address
{
    return {from.value()};
}

/// Return true if \p subject is within the given inclusive range.
template <u16 low, u16 high>
[[nodiscard]] constexpr auto in_range(Address subject) -> bool
{
    return low <= subject.value() && subject.value() <= high;
}

[[nodiscard]] constexpr auto operator==(Address lhs, Address rhs) -> bool
{
    return lhs.value() == rhs.value();
}

[[nodiscard]] constexpr auto operator!=(Address lhs, Address rhs) -> bool
{
    return !(lhs == rhs);
}

[[nodiscard]] constexpr auto operator+(Address lhs, Address rhs) -> Address
{
    return {(u16)(lhs.value() + rhs.value())};
}

[[nodiscard]] constexpr auto operator-(Address lhs, Address rhs) -> Address
{
    return {(u16)(lhs.value() - rhs.value())};
}

#endif  // GBEMU_ADDRESS_HPP
