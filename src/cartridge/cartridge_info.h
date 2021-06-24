#pragma once

#include <memory>
#include <string>
#include <vector>

#include "../definitions.hpp"

constexpr auto TITLE_LENGTH = 11;

namespace header {

constexpr auto entry_point       = 0x100;
constexpr auto logo              = 0x104;
constexpr auto title             = 0x134;
constexpr auto manufacturer_code = 0x13F;
constexpr auto cgb_flag          = 0x143;
constexpr auto new_license_code  = 0x144;
constexpr auto sgb_flag          = 0x146;
constexpr auto cartridge_type    = 0x147;
constexpr auto rom_size          = 0x148;
constexpr auto ram_size          = 0x149;
constexpr auto destination_code  = 0x14A;
constexpr auto old_license_code  = 0x14B;
constexpr auto version_number    = 0x14C;
constexpr auto header_checksum   = 0x14D;
constexpr auto global_checksum   = 0x14E;

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
