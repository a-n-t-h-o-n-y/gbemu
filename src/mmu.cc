#include "mmu.hpp"

#include "boot.hpp"
#include "cpu/cpu.hpp"
#include "input.hpp"
#include "serial.hpp"
#include "timer.hpp"
#include "util/bitwise.hpp"
#include "util/log.h"
#include "video/video.hpp"

MMU::MMU(Cartridge& inCartridge,
         CPU& inCPU,
         Video& inVideo,
         Input& inInput,
         Serial& inSerial,
         Timer& inTimer)
    : cartridge{inCartridge},
      cpu{inCPU},
      video{inVideo},
      input{inInput},
      serial{inSerial},
      timer{inTimer}
{
    memory = std::vector<u8>(0x10000);
}

auto MMU::read(Address const address) const -> u8
{
    /* VRAM */
    if (in_range<0x8000, 0x9FFF>(address))
        return memory_read(address);

    /* Mapped IO */
    if (in_range<0xFF00, 0xFF7F>(address))
        return read_io(address);

    if (in_range<0x0, 0x7FFF>(address)) {
        if (in_range<0x0, 0xFF>(address) && boot_rom_active())
            return bootDMG[address.value()];
        else
            return cartridge.read(address);
    }

    /* Zero Page ram */
    if (in_range<0xFF80, 0xFFFE>(address))
        return memory_read(address);

    /* External (cartridge) RAM */
    if (in_range<0xA000, 0xBFFF>(address))
        return cartridge.read(address);

    /* Internal work RAM */
    if (in_range<0xC000, 0xDFFF>(address))
        return memory_read(address);

    if (in_range<0xE000, 0xFDFF>(address))
        return memory_read(address - 0x2000);

    /* OAM */
    if (in_range<0xFE00, 0xFE9F>(address))
        return memory_read(address);

    if (in_range<0xFEA0, 0xFEFF>(address)) {
        log_warn("Attempting to read from unusable memory 0x%x",
                 address.value());
        return 0xFF;
    }

    /* Interrupt Enable register */
    if (address == 0xFFFF)
        return cpu.interrupt_enabled.value();

    fatal_error("Attempted to read from unmapped memory address 0x%X",
                address.value());
}

u8 MMU::memory_read(Address const address) const
{
    return memory.at(address.value());
}

auto MMU::read_io(Address const address) const -> u8
{
    switch (address.value()) {
        case 0xFF00: return input.get_input();
        case 0xFF01: return serial.read();
        case 0xFF02:
            log_unimplemented("Attempted to read serial transfer control");
            return 0xFF;
        case 0xFF04: return timer.get_divider();
        case 0xFF05: return timer.get_timer();
        case 0xFF06: return timer.get_timer_modulo();
        case 0xFF07: return timer.get_timer_control();
        case 0xFF0F: return cpu.interrupt_flag.value();

        /* TODO: Audio - Channel 1: Tone & Sweep */
        case 0xFF10:
        case 0xFF11:
        case 0xFF12:
        case 0xFF13:
        case 0xFF14: return 0xFF;

        /* TODO: Audio - Channel 2: Tone */
        case 0xFF16:
        case 0xFF17:
        case 0xFF18:
        case 0xFF19: return 0xFF;

        /* TODO: Audio - Channel 3: Wave Output */
        case 0xFF1A:
        case 0xFF1B:
        case 0xFF1C:
        case 0xFF1D:
        case 0xFF1E: return 0xFF;

        /* TODO: Audio - Channel 4: Noise */
        case 0xFF20:
        case 0xFF21:
        case 0xFF22:
        case 0xFF23: return 0xFF;

        /* TODO: Audio - Sound Control Registers */
        case 0xFF24:
            /* TODO */
            /* log_unimplemented("Read from channel control address 0x%x",
             * address.value()); */
            return 0xFF;

        case 0xFF25:
            /* TODO */
            return 0xFF;

        case 0xFF26:
            /* TODO */
            return 0xFF;

        /* TODO: Audio - Wave Pattern RAM */
        case 0xFF30:
        case 0xFF31:
        case 0xFF32:
        case 0xFF33:
        case 0xFF34:
        case 0xFF35:
        case 0xFF36:
        case 0xFF37:
        case 0xFF38:
        case 0xFF39:
        case 0xFF3A:
        case 0xFF3B:
        case 0xFF3C:
        case 0xFF3D:
        case 0xFF3E:
        case 0xFF3F: return memory_read(address);
        case 0xFF40: return video.control_byte;
        case 0xFF41: return video.lcd_status.value();
        case 0xFF42: return video.scroll_y.value();
        case 0xFF43: return video.scroll_x.value();
        case 0xFF44: return video.line.value();
        case 0xFF45: return video.ly_compare.value();
        case 0xFF47: return video.bg_palette.value();
        case 0xFF48: return video.sprite_palette_0.value();
        case 0xFF49: return video.sprite_palette_1.value();
        case 0xFF4A: return video.window_y.value();
        case 0xFF4B: return video.window_x.value();

        case 0xFF4D:
            log_unimplemented(
                "Attempted to read from 'Prepare Speed Switch' register");
            return 0x0;

        /* Disable boot rom switch */
        case 0xFF50: return memory_read(address);

        default:
            fatal_error("Read from unknown IO address 0x%x", address.value());
    }
}

void MMU::write(Address const address, u8 const byte)
{
    if (in_range<0x0000, 0x7FFF>(address)) {
        cartridge.write(address, byte);
        return;
    }

    /* VRAM */
    if (in_range<0x8000, 0x9FFF>(address)) {
        memory_write(address, byte);
        return;
    }

    /* External (cartridge) RAM */
    if (in_range<0xA000, 0xBFFF>(address)) {
        cartridge.write(address, byte);
        return;
    }

    /* Internal work RAM */
    if (in_range<0xC000, 0xDFFF>(address)) {
        memory_write(address, byte);
        return;
    }

    /* Mirrored RAM */
    if (in_range<0xE000, 0xFDFF>(address)) {
        log_warn("Attempting to write to mirrored work RAM");
        memory_write(address - 0x2000, byte);
        return;
    }

    /* OAM */
    if (in_range<0xFE00, 0xFE9F>(address)) {
        memory_write(address, byte);
        return;
    }

    if (in_range<0xFEA0, 0xFEFF>(address)) {
        log_warn("Attempting to write to unusable memory 0x%x - 0x%x",
                 address.value(), byte);
        return;
    }

    /* Mapped IO */
    if (in_range<0xFF00, 0xFF7F>(address)) {
        write_io(address, byte);
        return;
    }

    /* Zero Page ram */
    if (in_range<0xFF80, 0xFFFE>(address)) {
        memory_write(address, byte);
        return;
    }

    /* Interrupt Enable register */
    if (address == 0xFFFF) {
        cpu.interrupt_enabled.set(byte);
        return;
    }

    fatal_error("Attempted to write to unmapped memory address 0x%X",
                address.value());
}

void MMU::write_io(Address const address, u8 const byte)
{
    switch (address.value()) {
        case 0xFF00: input.write(byte); return;

        case 0xFF01:
            /* Serial data transfer (SB) */
            serial.write(byte);
            return;

        case 0xFF02:
            /* Serial data transfer (SB) */
            serial.write_control(byte);
            return;

        case 0xFF04: timer.reset_divider(); return;

        case 0xFF05:
            /* TODO: Time control */
            log_unimplemented("Wrote to timer counter");
            return;

        case 0xFF06: timer.set_timer_modulo(byte); return;

        case 0xFF07: timer.set_timer_control(byte); return;

        case 0xFF0F: cpu.interrupt_flag.set(byte); return;

        /* TODO: Audio - Channel 1: Tone & Sweep */
        case 0xFF10:
        case 0xFF11:
        case 0xFF12:
        case 0xFF13:
        case 0xFF14: return;

        /* TODO: Audio - Channel 2: Tone */
        case 0xFF16:
        case 0xFF17:
        case 0xFF18:
        case 0xFF19: return;

        /* TODO: Audio - Channel 3: Wave Output */
        case 0xFF1A:
        case 0xFF1B:
        case 0xFF1C:
        case 0xFF1D:
        case 0xFF1E: return;

        /* TODO: Audio - Channel 4: Noise */
        case 0xFF20:
        case 0xFF21:
        case 0xFF22:
        case 0xFF23: return;

        /* TODO: Audio - Sound Control Registers */
        case 0xFF24:
            /* TODO */
            /* log_unimplemented("Wrote to channel control address 0x%x - 0x%x",
             * address.value(), byte); */
            return;

        case 0xFF25:
            /* TODO */
            /* log_unimplemented("Wrote to selection of sound output terminal
             * address 0x%x - 0x%x", address.value(), byte); */
            return;

        case 0xFF26:
            /* TODO */
            log_unimplemented("Wrote to sound on/off address 0x%x - 0x%x",
                              address.value(), byte);
            return;

        /* TODO: Audio - Wave Pattern RAM */
        case 0xFF30:
        case 0xFF31:
        case 0xFF32:
        case 0xFF33:
        case 0xFF34:
        case 0xFF35:
        case 0xFF36:
        case 0xFF37:
        case 0xFF38:
        case 0xFF39:
        case 0xFF3A:
        case 0xFF3B:
        case 0xFF3C:
        case 0xFF3D:
        case 0xFF3E:
        case 0xFF3F: memory_write(address, byte); return;

        /* Switch on LCD */
        case 0xFF40: video.control_byte = byte; return;

        case 0xFF41:
            /* TODO */
            video.lcd_status.set(byte);
            return;

        /* Vertical Scroll Register */
        case 0xFF42: video.scroll_y.set(byte); return;

        /* Horizontal Scroll Register */
        case 0xFF43: video.scroll_x.set(byte); return;

        /* LY - Line Y coordinate */
        case 0xFF44:
            /* "Writing will reset the counter */
            video.line.set(0x0);
            return;

        case 0xFF45: video.ly_compare.set(byte); return;

        case 0xFF46: dma_transfer(byte); return;

        case 0xFF47:
            video.bg_palette.set(byte);
            log_trace("Set video palette: 0x%x", byte);
            return;

        case 0xFF48:
            video.sprite_palette_0.set(byte);
            log_trace("Set sprite palette 0: 0x%x", byte);
            return;

        case 0xFF49:
            video.sprite_palette_1.set(byte);
            log_trace("Set sprite palette 1: 0x%x", byte);
            return;

        case 0xFF4A: video.window_y.set(byte); return;

        case 0xFF4B: video.window_x.set(byte); return;

        case 0xFF4D:
            log_unimplemented(
                "Attempted to write to 'Prepare Speed Switch' register");
            return;

        /* Disable boot rom switch */
        case 0xFF50:
            memory_write(address, byte);
            global_logger.enable_tracing();
            log_debug("Boot rom was disabled");
            return;

        case 0xFF7F:
            log_warn("Attempt to write to unused memory 0x%x", address.value());
            return;

        default:
            /* TODO */
            fatal_error("Wrote 0x%x to unknown address 0x%x", byte,
                        address.value());
    }
}

void MMU::memory_write(Address const address, u8 const byte)
{
    memory.at(address.value()) = byte;
}

bool MMU::boot_rom_active() const { return read(0xFF50) != 0x1; }

void MMU::dma_transfer(u8 const byte)
{
    Address start_address = byte * 0x100;

    for (u8 i = 0x0; i <= 0x9F; ++i) {
        Address from_address = start_address.value() + i;
        Address to_address   = 0xFE00 + i;

        u8 value_at_address = read(from_address);
        write(to_address, value_at_address);
    }
}
