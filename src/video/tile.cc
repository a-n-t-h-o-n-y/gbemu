#include "tile.hpp"

#include <array>

#include "../util/bitwise.hpp"
#include "color.hpp"

using bitwise::bit_value;

Tile::Tile(Address tile_address, MMU& mmu, uint size_multiplier)
{
    for (uint tile_line = 0; tile_line < TILE_HEIGHT_PX * size_multiplier;
         ++tile_line) {
        /* 2 (bytes per line of pixels) * y (lines) */
        uint index_into_tile = 2 * tile_line;
        Address line_start   = tile_address + index_into_tile;

        u8 pixels_1 = mmu.read(line_start);
        u8 pixels_2 = mmu.read(line_start + 1);

        auto const pixel_line = get_pixel_line(pixels_1, pixels_2);
        for (uint x = 0; x < TILE_WIDTH_PX; ++x)
            buffer_[pixel_index(x, tile_line)] = get_color(pixel_line[x]);
    }
}

auto Tile::get_pixel(uint x, uint y) const -> GBColor
{
    return buffer_[pixel_index(x, y)];
}

auto Tile::get_pixel_line(u8 byte1, u8 byte2) const -> std::array<u8, 8>
{
    using bitwise::bit_value;
    auto pixel_line = std::array<u8, 8>{};
    for (u8 i = 0; i < 8; ++i) {
        auto const color_value = static_cast<u8>(
            (bit_value(byte2, 7 - i) << 1) | bit_value(byte1, 7 - i));
        pixel_line[i] = color_value;
    }
    return pixel_line;
}

auto Tile::pixel_index(uint x, uint y) -> uint
{
    return (y * TILE_HEIGHT_PX) + x;
}
