#ifndef GBEMU_FRAMEBUFFER_HPP
#define GBEMU_FRAMEBUFFER_HPP
#include <algorithm>
#include <array>

#include "../definitions.hpp"

template <uint width, uint height>
class FrameBuffer_impl {
   public:
    void set_pixel(uint x, uint y, Color color)
    {
        buffer_[pixel_index(x, y)] = color;
    }

    [[nodiscard]] auto get_pixel(uint x, uint y) const -> Color
    {
        return buffer_[pixel_index(x, y)];
    }

    void reset()
    {
        std::fill(std::begin(buffer_), std::end(buffer_), Color::White);
    }

   private:
    std::array<Color, GAMEBOY_WIDTH * GAMEBOY_HEIGHT> buffer_;

   private:
    [[nodiscard]] constexpr static auto pixel_index(uint x, uint y) -> uint
    {
        return (y * width) + x;
    }
};

using FrameBuffer = FrameBuffer_impl<GAMEBOY_WIDTH, GAMEBOY_HEIGHT>;

#endif  // GBEMU_FRAMEBUFFER_HPP
