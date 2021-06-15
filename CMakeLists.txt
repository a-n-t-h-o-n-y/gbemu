cmake_minimum_required(VERSION 3.0)
project(gbemu)

list(APPEND CMAKE_MODULE_PATH
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/platforms/sfml
)

include(add_warnings)
include(add_sources)


add_definitions(-std=c++17)
add_warnings()

declare_library(gbemu-core src)

set(PLATFORM "SDL2" CACHE STRING "Set Frontend")
message("Platform: ${PLATFORM}")

# SFML target
if (PLATFORM STREQUAL "SFML")
  find_package(SFML 2 COMPONENTS system window graphics)
  if (SFML_FOUND)
    declare_executable(gbemu-sfml platforms/sfml)
    include_directories(SYSTEM ${SFML_INCLUDE_DIR})
    target_link_libraries(gbemu-sfml gbemu-core ${SFML_LIBRARIES})
  else()
    message("Error: Could not find SFML")
  endif()
endif()

# SDL target
if (PLATFORM STREQUAL "SDL2")
  find_package(SDL2)
  if (SDL2_FOUND)
    declare_executable(gbemu platforms/sdl)
    include_directories(SYSTEM ${SDL2_INCLUDE_DIRS})
    target_link_libraries(gbemu gbemu-core ${SDL2_LIBRARIES})
  endif()
endif()

# TermOx target
if (PLATFORM STREQUAL "TERMOX")
  add_subdirectory(external/TermOx)
  declare_executable(gbemu platforms/termox)
  target_link_libraries(gbemu gbemu-core termox)
endif()

# Test target
declare_executable(gbemu-test platforms/test)
target_link_libraries(gbemu-test gbemu-core)

