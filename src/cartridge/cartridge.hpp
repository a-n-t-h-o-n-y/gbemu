#ifndef GBEMU_CARTRIDGE_CARTRIDGE_HPP
#define GBEMU_CARTRIDGE_CARTRIDGE_HPP
#include <memory>
#include <vector>

#include "../address.hpp"
#include "../register.hpp"
#include "cartridge_info.h"

class Cartridge {
   public:
    Cartridge(std::vector<u8> rom_data,
              std::vector<u8> ram_data,
              CartridgeInfo cartridge_info);

    virtual ~Cartridge() = default;

   public:
    [[nodiscard]] virtual auto read(Address address) const -> u8 = 0;

    virtual void write(Address address, u8 value) = 0;

   public:
    [[nodiscard]] auto get_cartridge_ram() const -> std::vector<u8> const&;

   protected:
    std::vector<u8> rom;
    std::vector<u8> ram;
    CartridgeInfo cartridge_info;
};

[[nodiscard]] auto get_cartridge(std::vector<u8> rom_data,
                                 std::vector<u8> ram_data = {})
    -> std::unique_ptr<Cartridge>;

class NoMBC : public Cartridge {
   public:
    NoMBC(std::vector<u8> rom_data,
          std::vector<u8> ram_data,
          CartridgeInfo cartridge_info);

   public:
    [[nodiscard]] auto read(Address address) const -> u8 override;

    void write(Address address, u8 value) override;
};

class MBC1 : public Cartridge {
   public:
    MBC1(std::vector<u8> rom_data,
         std::vector<u8> ram_data,
         CartridgeInfo cartridge_info);

   public:
    [[nodiscard]] auto read(Address address) const -> u8 override;

    void write(Address address, u8 value) override;

   private:
    WordRegister rom_bank;
    WordRegister ram_bank;
    bool ram_enabled = false;

    // TODO: ROM/RAM Mode Select (6000-7FFF)
    // This 1bit Register selects whether the two bits of the above register
    // should be used as upper two bits of the ROM Bank, or as RAM Bank Number.
    bool rom_banking_mode = true;
};

class MBC3 : public Cartridge {
   public:
    MBC3(std::vector<u8> rom_data,
         std::vector<u8> ram_data,
         CartridgeInfo cartridge_info);

   public:
    [[nodiscard]] auto read(Address address) const -> u8 override;

    void write(Address address, u8 value) override;

   private:
    WordRegister rom_bank;
    WordRegister ram_bank;
    bool ram_enabled  = false;
    bool ram_over_rtc = true;

    // TODO: ROM/RAM Mode Select (6000-7FFF)
    // This 1bit Register selects whether the two bits of the above register
    // should be used as upper two bits of the ROM Bank, or as RAM Bank Number.
    bool rom_banking_mode = true;
};

#endif  // GBEMU_CARTRIDGE_CARTRIDGE_HPP
