#pragma once

#include "cpu/cpu.h"
#include "debugger.h"
#include "input.h"
#include "options.h"
#include "serial.h"
#include "timer.h"
#include "util/log.h"
#include "video/video.h"

#include <functional>
#include <memory>
#include <utility>

typedef std::function<bool(void)> should_close_callback_t;

class Gameboy {
public:
    Gameboy(std::vector<u8> cartridge_data, Options& options, std::vector<u8> save_data = {});

    void run(const should_close_callback_t& _should_close_callback,
             const vblank_callback_t& _vblank_callback);

    void button_pressed(GbButton button);
    void button_released(GbButton button);

    void debug_toggle_background();
    void debug_toggle_sprites();
    void debug_toggle_window();

    const std::vector<u8>& get_cartridge_ram() const;

    void tick();

    template <typename F> void register_draw_callback(F&& fn) {
        video.register_vblank_callback(std::forward<F>(fn));
    }

private:
    std::shared_ptr<Cartridge> cartridge;
    Input input;
    CPU cpu;
    Video video;
    Serial serial;
    MMU mmu;
    Timer timer;

    Debugger debugger;

    friend class Debugger;

    uint elapsed_cycles = 0;

    should_close_callback_t should_close_callback;
};
