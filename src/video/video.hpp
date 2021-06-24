#ifndef GBEMU_VIDEO_VIDEO_HPP
#define GBEMU_VIDEO_VIDEO_HPP
#include <functional>
#include <memory>
#include <vector>

#include "../definitions.hpp"
#include "../mmu.hpp"
#include "../options.hpp"
#include "../register.hpp"
#include "framebuffer.hpp"
#include "tile.hpp"

using VBlank_callback_t = std::function<void(FrameBuffer const&)>;

enum class VideoMode { ACCESS_OAM, ACCESS_VRAM, HBLANK, VBLANK };

class Video {
   public:
    u8 control_byte;

    FlagRegister lcd_control;
    FlagRegister lcd_status;

    FlagRegister scroll_y;
    FlagRegister scroll_x;

    /* LCDC Y-coordinate */
    FlagRegister line; /* Line y-position: register LY */
    FlagRegister ly_compare;

    FlagRegister window_y;
    FlagRegister window_x; /* Note: x - 7 */

    FlagRegister bg_palette;
    FlagRegister sprite_palette_0; /* OBP0 */
    FlagRegister sprite_palette_1; /* OBP1 */

    /* TODO: LCD Color Palettes (CGB) */
    /* TODO: LCD VRAM Bank (CGB) */

    FlagRegister dma_transfer; /* DMA */

    bool debug_disable_background = false;
    bool debug_disable_sprites    = false;
    bool debug_disable_window     = false;

   public:
    Video(CPU& inCPU, MMU& inMMU, Options& inOptions);

   public:
    void tick(Cycles cycles);

    void register_vblank_callback(const VBlank_callback_t& _vblank_callback);

   private:
    void write_scanline(u8 current_line);

    void write_sprites();

    void draw();

    void draw_bg_line(uint current_line);

    void draw_window_line(uint current_line);

    void draw_sprite(uint sprite_n);

    auto get_pixel_from_line(u8 byte1, u8 byte2, u8 pixel_index) const
        -> GBColor;

    auto is_on_screen(u8 x, u8 y) const -> bool;

    auto is_on_screen_x(u8 x) const -> bool;

    auto is_on_screen_y(u8 y) const -> bool;

    auto display_enabled() const -> bool;

    auto window_tile_map() const -> bool;

    auto window_enabled() const -> bool;

    auto bg_window_tile_data() const -> bool;

    auto bg_tile_map_display() const -> bool;

    auto sprite_size() const -> bool;

    auto sprites_enabled() const -> bool;

    auto bg_enabled() const -> bool;

    auto get_real_color(u8 pixel_value) const -> Color;

    auto load_palette(FlagRegister palette_register) const -> Palette;

    auto get_color_from_palette(GBColor color, const Palette& palette) -> Color;

   private:
    CPU& cpu;
    MMU& mmu;
    FrameBuffer buffer;

    VideoMode current_mode = VideoMode::ACCESS_OAM;
    uint cycle_counter     = 0;

    VBlank_callback_t vblank_callback;
};

#endif  // GBEMU_VIDEO_VIDEO_HPP
