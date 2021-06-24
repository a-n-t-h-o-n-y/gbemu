#ifndef GBEMU_MMU_HPP
#define GBEMU_MMU_HPP
#include <vector>

#include "address.hpp"
#include "cartridge/cartridge.hpp"

class Video;
class CPU;
class Serial;
class Input;
class Timer;

class MMU {
   public:
    MMU(Cartridge& inCartridge,
        CPU& inCPU,
        Video& inVideo,
        Input& input,
        Serial& serial,
        Timer& timer);

   public:
    [[nodiscard]] auto read(Address address) const -> u8;

    void write(Address address, u8 byte);

   private:
    [[nodiscard]] auto boot_rom_active() const -> bool;

    [[nodiscard]] auto read_io(Address address) const -> u8;

    void write_io(Address address, u8 byte);

    [[nodiscard]] auto memory_read(Address address) const -> u8;

    void memory_write(Address address, u8 byte);

    void dma_transfer(u8 byte);

   private:
    Cartridge& cartridge;
    CPU& cpu;
    Video& video;
    Input& input;
    Serial& serial;
    Timer& timer;

    std::vector<u8> memory;
};

#endif  // GBEMU_MMU_HPP
