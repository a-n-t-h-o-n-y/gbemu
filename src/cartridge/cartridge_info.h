#pragma once

#include "../definitions.h"

#include <memory>
#include <string>
#include <vector>

const int TITLE_LENGTH = 11;

namespace header {

auto const entry_point       = 0x100;
auto const logo              = 0x104;
auto const title             = 0x134;
auto const manufacturer_code = 0x13F;
auto const cgb_flag          = 0x143;
auto const new_license_code  = 0x144;
auto const sgb_flag          = 0x146;
auto const cartridge_type    = 0x147;
auto const rom_size          = 0x148;
auto const ram_size          = 0x149;
auto const destination_code  = 0x14A;
auto const old_license_code  = 0x14B;
auto const version_number    = 0x14C;
auto const header_checksum   = 0x14D;
auto const global_checksum   = 0x14E;

}  // namespace header

enum class CartridgeType {
    ROMOnly,
    MBC1,
    MBC2,
    MBC3,
    MBC4,
    MBC5,
    Unknown,
};

extern CartridgeType get_type(u8 type);
extern std::string describe(CartridgeType type);

extern std::string get_title(std::vector<u8>& rom);

extern std::string get_license(u16 old_license, u16 new_license);

enum class ROMSize {
    KB32,
    KB64,
    KB128,
    KB256,
    KB512,
    MB1,
    MB2,
    MB4,
    MB1p1,
    MB1p2,
    MB1p5,
};

extern ROMSize get_rom_size(u8 size_code);
extern std::string describe(ROMSize size);

enum class RAMSize {
    None,
    KB2,
    KB8,
    KB32,
    KB128,
    KB64,
};

extern RAMSize get_ram_size(u8 size_code);
extern uint get_expected_ram_size(RAMSize size_code);
extern std::string describe(RAMSize size);

enum class Destination {
    Japanese,
    NonJapanese,
};

extern Destination get_destination(u8 destination);
extern std::string describe(Destination destination);

class CartridgeInfo {
   public:
    std::string title;

    /* Cartridge information */
    CartridgeType type;
    Destination destination;
    ROMSize rom_size;
    RAMSize ram_size;
    std::string license_code;
    u8 version;

    u16 header_checksum;
    u16 global_checksum;

    bool supports_cgb;
    bool supports_sgb;
};

auto get_info(std::vector<u8> rom) -> CartridgeInfo;
