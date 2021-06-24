#include "cartridge.hpp"

#include <memory>
#include <utility>
#include <vector>

#include "../address.hpp"
#include "../util/files.hpp"
#include "../util/log.h"

Cartridge::Cartridge(std::vector<u8> rom_data,
                     std::vector<u8> ram_data,
                     CartridgeInfo cartridge_info)
    : rom(std::move(rom_data)), cartridge_info(std::move(cartridge_info))
{
    auto const expected_size = get_expected_ram_size(cartridge_info.ram_size);

    if (ram_data.size() == expected_size)
        ram = ram_data;
    else if (ram_data.size() == 0)
        ram = std::vector<u8>(expected_size, 0);
    else {
        fatal_error("Invalid or corrupted RAM file. Read %d bytes, expected %d",
                    ram_data.size(), expected_size);
    }
}

auto Cartridge::get_cartridge_ram() const -> std::vector<u8> const&
{
    return ram;
}

auto get_cartridge(std::vector<u8> rom_data, std::vector<u8> ram_data)
    -> std::unique_ptr<Cartridge>
{
    auto info = get_info(rom_data);
    switch (info.type) {
        case CartridgeType::ROMOnly:
            return std::make_unique<NoMBC>(rom_data, ram_data, std::move(info));
        case CartridgeType::MBC1:
            return std::make_unique<MBC1>(rom_data, ram_data, std::move(info));
        case CartridgeType::MBC2: fatal_error("MBC2 is unimplemented");
        case CartridgeType::MBC3:
            return std::make_unique<MBC3>(rom_data, ram_data, std::move(info));
        case CartridgeType::MBC4: fatal_error("MBC4 is unimplemented");
        case CartridgeType::MBC5: fatal_error("MBC5 is unimplemented");
        case CartridgeType::Unknown: fatal_error("Unknown cartridge type");
    }
}

NoMBC::NoMBC(std::vector<u8> rom_data,
             std::vector<u8> ram_data,
             CartridgeInfo cartridge_info)
    : Cartridge(rom_data, ram_data, std::move(cartridge_info))
{}

void NoMBC::write(Address, u8)
{
    log_warn("Attempting to write to cartridge ROM without an MBC");
    return;
}

u8 NoMBC::read(Address address) const
{
    /* TODO: check this address is in sensible bounds */
    return rom.at(address.value());
}

MBC1::MBC1(std::vector<u8> rom_data,
           std::vector<u8> ram_data,
           CartridgeInfo cartridge_info)
    : Cartridge(rom_data, ram_data, std::move(cartridge_info))
{
    unused(rom_banking_mode);
    rom_bank.set(0x1);
}

void MBC1::write(Address address, u8 value)
{
    if (in_range<0x0000, 0x1FFF>(address))
        ram_enabled = true;

    if (in_range<0x2000, 0x3FFF>(address)) {
        if (value == 0x0)
            rom_bank.set(0x1);
        else if (value == 0x20) {
            rom_bank.set(0x21);
            return;
        }
        else if (value == 0x40) {
            rom_bank.set(0x41);
            return;
        }
        else if (value == 0x60) {
            rom_bank.set(0x61);
            return;
        }

        u16 rom_bank_bits = value & 0x1F;
        rom_bank.set(rom_bank_bits);
    }

    if (in_range<0x4000, 0x5FFF>(address)) {
        log_unimplemented(
            "Unimplemented: Setting upper bits of ROM bank number");
    }

    if (in_range<0x6000, 0x7FFF>(address)) {
        log_unimplemented("Unimplemented: Selecting ROM/RAM Mode");
    }

    if (in_range<0xA000, 0xBFFF>(address)) {
        if (!ram_enabled)
            return;

        auto offset_into_ram           = 0x2000 * ram_bank.value();
        auto address_in_ram            = (address - 0xA000) + offset_into_ram;
        ram.at(address_in_ram.value()) = value;
    }
}

auto MBC1::read(Address address) const -> u8
{
    if (in_range<0x0000, 0x3FFF>(address))
        return rom.at(address.value());
    else if (in_range<0x4000, 0x7FFF>(address)) {
        u16 address_into_bank = address.value() - 0x4000;
        uint bank_offset      = 0x4000 * rom_bank.value();

        uint address_in_rom = bank_offset + address_into_bank;
        return rom.at(address_in_rom);
    }
    else if (in_range<0xA000, 0xBFFF>(address)) {
        auto offset_into_ram = 0x2000 * ram_bank.value();
        auto address_in_ram  = (address - 0xA000) + offset_into_ram;
        return ram.at(address_in_ram.value());
    }
    fatal_error("Attempted to read from unmapped MBC1 address 0x%x",
                address.value());
}

MBC3::MBC3(std::vector<u8> rom_data,
           std::vector<u8> ram_data,
           CartridgeInfo cartridge_info)
    : Cartridge(rom_data, ram_data, std::move(cartridge_info))
{
    unused(rom_banking_mode);
    rom_bank.set(0x1);
}

void MBC3::write(Address address, u8 value)
{
    if (in_range<0x0000, 0x1FFF>(address)) {
        if (value == 0x0A)
            ram_enabled = true;
        else if (value == 0x0)
            ram_enabled = false;
    }

    if (in_range<0x2000, 0x3FFF>(address)) {
        if (value == 0x0)
            rom_bank.set(0x1);
        u16 rom_bank_bits = value & 0x7F;
        rom_bank.set(rom_bank_bits);
    }

    if (in_range<0x4000, 0x5FFF>(address)) {
        if (value <= 0x03) {
            ram_over_rtc = true;
            ram_bank.set(value);
        }
        else if (value >= 0x08 && value <= 0xC) {
            ram_over_rtc = false;
            log_unimplemented("Using RTC registers of MBC3 cartridge");
        }
    }

    if (in_range<0x6000, 0x7FFF>(address)) {
        log_unimplemented("Unimplemented: Latch clock data");
    }

    if (in_range<0xA000, 0xBFFF>(address)) {
        if (!ram_enabled)
            return;
        if (ram_over_rtc) {
            auto const offset_into_ram = 0x2000 * ram_bank.value();
            auto const address_in_ram  = (address - 0xA000) + offset_into_ram;
            ram.at(address_in_ram.value()) = value;
        }
    }
}

u8 MBC3::read(Address address) const
{
    if (in_range<0x0000, 0x3FFF>(address))
        return rom.at(address.value());
    else if (in_range<0x4000, 0x7FFF>(address)) {
        u16 address_into_bank = address.value() - 0x4000;
        uint bank_offset      = 0x4000 * rom_bank.value();

        uint address_in_rom = bank_offset + address_into_bank;
        return rom.at(address_in_rom);
    }
    else if (in_range<0xA000, 0xBFFF>(address)) {
        auto const offset_into_ram = 0x2000 * ram_bank.value();
        auto const address_in_ram  = (address - 0xA000) + offset_into_ram;
        return ram.at(address_in_ram.value());
    }

    fatal_error("Attempted to read from unmapped MBC1 address 0x%x",
                address.value());
}
