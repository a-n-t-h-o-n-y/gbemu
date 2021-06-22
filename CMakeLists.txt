cmake_minimum_required(VERSION 3.9)
project(gbemu)

add_library(gbemu-core
    src/address.cc
    src/debugger.cc
    src/gameboy.cc
    src/input.cc
    src/mmu.cc
    src/register.cc
    src/serial.cc
    src/timer.cc
    src/cartridge/cartridge.cc
    src/cartridge/cartridge_info.cc
    src/cpu/cpu.cc
    src/cpu/opcode_mapping.cc
    src/cpu/opcodes.cc
    src/util/files.cc
    src/util/log.cc
    src/util/string_utils.cc
    src/video/color.cc
    src/video/framebuffer.cc
    src/video/tile.cc
    src/video/video.cc
)

target_include_directories(gbemu-core
    PUBLIC
        ${PROJECT_SOURCE_DIR}/src
)

target_compile_features(gbemu-core
    PUBLIC
        cxx_std_17
)

target_compile_options(gbemu-core
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)

# set(PLATFORM "SDL2" CACHE STRING "Set Frontend")
# message("Platform: ${PLATFORM}")

# # SFML target
# if (PLATFORM STREQUAL "SFML")
#   find_package(SFML 2 COMPONENTS system window graphics)
#   if (SFML_FOUND)
#     declare_executable(gbemu-sfml platforms/sfml)
#     include_directories(SYSTEM ${SFML_INCLUDE_DIR})
#     target_link_libraries(gbemu-sfml gbemu-core ${SFML_LIBRARIES})
#   else()
#     message("Error: Could not find SFML")
#   endif()
# endif()

# # SDL target
# if (PLATFORM STREQUAL "SDL2")
#   find_package(SDL2)
#   if (SDL2_FOUND)
#     declare_executable(gbemu platforms/sdl)
#     include_directories(SYSTEM ${SDL2_INCLUDE_DIRS})
#     target_link_libraries(gbemu gbemu-core ${SDL2_LIBRARIES})
#   endif()
# endif()

# Test target
# declare_executable(gbemu-test platforms/test)
# target_link_libraries(gbemu-test gbemu-core)
