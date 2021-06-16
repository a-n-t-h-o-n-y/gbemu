#pragma once

#include "../definitions.h"

#include <vector>

class FrameBuffer {
public:
    FrameBuffer() = default;

    FrameBuffer(uint width, uint height);

    void set_pixel(uint x, uint y, Color color);
    Color get_pixel(uint x, uint y) const;

    void reset();

private:
    uint width = 0;
    uint height = 0;

    uint pixel_index(uint x, uint y) const;

    std::vector<Color> buffer;
};
