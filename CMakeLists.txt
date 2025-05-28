# `add_subdirectory(SYSTEM)`: 3.25
cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

# Allow variables to propagate to sub-projects.
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
cmake_policy(SET CMP0077 NEW)

# Enforce IPO when set.
set(CMAKE_POLICY_DEFAULT_CMP0069 NEW)
cmake_policy(SET CMP0069 NEW)

# Enable IPO globally if supported.
include(CheckIPOSupported)
check_ipo_supported(RESULT IPO_SUPPORTED)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ${IPO_SUPPORTED})

project(tins)

include(${CMAKE_CURRENT_LIST_DIR}/cmake/utilities.cmake)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/vendor)

add_executable(game WIN32)

target_sources(game PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/main.c)
target_include_directories(game PRIVATE include)

target_link_libraries(game PRIVATE allegro)
