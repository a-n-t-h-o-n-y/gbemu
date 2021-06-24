#ifndef GBEMU_VIDEO_TILE_HPP
#define GBEMU_VIDEO_TILE_HPP
#include <array>

#include "../address.hpp"
#include "../definitions.hpp"
#include "../mmu.hpp"

constexpr auto TILES_PER_LINE = uint{32};
constexpr auto TILE_HEIGHT_PX = uint{8};
constexpr auto TILE_WIDTH_PX  = uint{8};

constexpr auto TILE_SET_ZERO_ADDRESS = Address{0x8000};
constexpr auto TILE_SET_ONE_ADDRESS  = Address{0x8800};

constexpr auto TILE_MAP_ZERO_ADDRESS = Address{0x9800};
constexpr auto TILE_MAP_ONE_ADDRESS  = Address{0x9C00};

/* A single tile contains 8 lines, each of which is two bytes */
constexpr auto TILE_BYTES = uint{2 * 8};

constexpr auto SPRITE_BYTES = uint{4};

class Tile {
   public:
    Tile(Address address, MMU& mmu, uint size_multiplier = 1);

    [[nodiscard]] auto get_pixel(uint x, uint y) const -> GBColor;

   private:
    std::array<GBColor, TILE_HEIGHT_PX * 2 * TILE_WIDTH_PX> buffer_;

   private:
    [[nodiscard]] static auto pixel_index(uint x, uint y) -> uint;

    [[nodiscard]] auto get_pixel_line(u8 byte1, u8 byte2) const
        -> std::array<u8, 8>;
};
#endif  // GBEMU_VIDEO_TILE_HPP
