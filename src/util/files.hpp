#ifndef GBEMU_UTIL_FILES_HPP
#define GBEMU_UTIL_FILES_HPP
#include <string>
#include <vector>

#include "../definitions.hpp"

auto read_bytes(std::string const& filename) -> std::vector<u8>;

#endif  // GBEMU_UTIL_FILES_HPP
