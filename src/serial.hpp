#ifndef GBEMU_SERIAL_HPP
#define GBEMU_SERIAL_HPP
#include "definitions.hpp"
#include "options.hpp"

class Serial {
   public:
    Serial(Options& inOptions) : options(inOptions) {}

   public:
    auto read() const -> u8;

    void write(u8 byte);

    void write_control(u8 byte);

   private:
    Options& options;
    u8 data;
};

#endif  // GBEMU_SERIAL_HPP
