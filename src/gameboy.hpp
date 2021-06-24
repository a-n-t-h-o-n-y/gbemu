#ifndef GBEMU_GAMEBOY_HPP
#define GBEMU_GAMEBOY_HPP
#include <functional>
#include <utility>

#include "cpu/cpu.hpp"
#include "input.hpp"
#include "options.hpp"
#include "serial.hpp"
#include "timer.hpp"
#include "util/log.h"
#include "video/video.hpp"

using Should_close_callback_t = std::function<bool(void)>;

class Gameboy {
   public:
    Gameboy(Cartridge& cart, Options& options);

    void run(const Should_close_callback_t& _should_close_callback,
             const VBlank_callback_t& _vblank_callback);

    void button_pressed(GbButton button);
    void button_released(GbButton button);

    void debug_toggle_background();
    void debug_toggle_sprites();
    void debug_toggle_window();

    const std::vector<u8>& get_cartridge_ram() const;

    void tick();

    template <typename F>
    void register_draw_callback(F&& fn)
    {
        video.register_vblank_callback(std::forward<F>(fn));
    }

   private:
    Cartridge& cartridge;
    Input input;
    CPU cpu;
    Video video;
    Serial serial;
    MMU mmu;
    Timer timer;

    Should_close_callback_t should_close_callback;
};

#endif  // GBEMU_GAMEBOY_HPP
