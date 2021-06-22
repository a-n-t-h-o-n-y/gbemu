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
    auto read(Address address) const -> u8;

    void write(Address address, u8 byte);

   private:
    bool boot_rom_active() const;

    u8 read_io(const Address& address) const;

    void write_io(const Address& address, u8 byte);

    u8 memory_read(const Address& address) const;

    void memory_write(const Address& address, u8 byte);

    void dma_transfer(const u8 byte);

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
