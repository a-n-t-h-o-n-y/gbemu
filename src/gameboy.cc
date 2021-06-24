#include "gameboy.hpp"

Gameboy::Gameboy(Cartridge& cart, Options& options)
    : cartridge{cart},
      cpu{mmu, options},
      video{cpu, mmu, options},
      serial{options},
      mmu{cartridge, cpu, video, input, serial, timer}
{
    if (options.disable_logs)
        log_set_level(LogLevel::Error);

    log_set_level(options.trace ? LogLevel::Trace : LogLevel::Info);
}

void Gameboy::button_pressed(GbButton button) { input.button_pressed(button); }

void Gameboy::button_released(GbButton button)
{
    input.button_released(button);
}

void Gameboy::debug_toggle_background()
{
    video.debug_disable_background = !video.debug_disable_background;
}

void Gameboy::debug_toggle_sprites()
{
    video.debug_disable_sprites = !video.debug_disable_sprites;
}

void Gameboy::debug_toggle_window()
{
    video.debug_disable_window = !video.debug_disable_window;
}

void Gameboy::run(const Should_close_callback_t& _should_close_callback,
                  const VBlank_callback_t& _vblank_callback)
{
    should_close_callback = _should_close_callback;

    video.register_vblank_callback(_vblank_callback);

    while (!should_close_callback())
        tick();
}

void Gameboy::tick()
{
    auto const cycles = cpu.tick();
    video.tick(cycles);
    // timer.tick(cycles); // TODO Not Implemented Yet
}

const std::vector<u8>& Gameboy::get_cartridge_ram() const
{
    return cartridge.get_cartridge_ram();
}
