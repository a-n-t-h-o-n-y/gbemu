#include "files.hpp"

#include <fstream>
#include <vector>

#include "log.h"

auto read_bytes(const std::string& filename) -> std::vector<u8>
{
    auto stream =
        std::ifstream{filename.c_str(), std::ios::binary | std::ios::ate};

    if (!stream.good()) {
        fatal_error("Cannot read from file: %s", filename.c_str());
    }

    auto const position  = stream.tellg();
    auto const file_size = static_cast<std::size_t>(position);

    auto file_contents = std::vector<char>(file_size);

    stream.seekg(0, std::ios::beg);
    stream.read(&file_contents[0], static_cast<std::streamsize>(position));
    stream.close();

    return std::vector<u8>(file_contents.begin(), file_contents.end());
}
