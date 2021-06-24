#ifndef GBEMU_DEFINITIONS_HPP
#define GBEMU_DEFINITIONS_HPP
#include <cstdint>

#include "../src/util/log.h"

using uint = unsigned int;

using u8  = std::uint8_t;
using u16 = std::uint16_t;
using s8  = std::int8_t;
using s16 = std::uint16_t;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
template <typename... T>
void unused(T&&... unused_vars)
{}
#pragma clang diagnostic pop

#define fatal_error(...)                                                    \
    log_error("Fatal error @ %s (line %d)", __PRETTY_FUNCTION__, __LINE__); \
    log_error(__VA_ARGS__);                                                 \
    exit(1);

constexpr auto GAMEBOY_WIDTH  = uint{160};
constexpr auto GAMEBOY_HEIGHT = uint{144};
constexpr auto BG_MAP_SIZE    = uint{256};

constexpr auto CLOCK_RATE = 4'194'304;

enum class GBColor : u8 {
    Color0, /* White */
    Color1, /* Light gray */
    Color2, /* Dark gray */
    Color3  /* Black */
};

enum class Color : u8 { White, LightGray, DarkGray, Black };

struct Palette {
    Color color0 = Color::White;
    Color color1 = Color::LightGray;
    Color color2 = Color::DarkGray;
    Color color3 = Color::Black;
};

using Cycles = uint;

#endif  // GBEMU_DEFINITIONS_HPP
