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

find_library(MATH_LIBRARY m)

include(${CMAKE_CURRENT_LIST_DIR}/cmake/utilities.cmake)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/vendor)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/tool)

add_executable(game WIN32)
target_compile_features(game PRIVATE c_std_23)

cxx_default_diagnostics(game)

target_link_libraries(game PRIVATE allegro_monolith nuklear)

target_include_directories(game PRIVATE include)
target_sources(
		game PRIVATE
		${CMAKE_CURRENT_LIST_DIR}/src/main.c
		${CMAKE_CURRENT_LIST_DIR}/src/result.c
		${CMAKE_CURRENT_LIST_DIR}/src/log.c
		${CMAKE_CURRENT_LIST_DIR}/src/display.c
		${CMAKE_CURRENT_LIST_DIR}/src/settings.c
		${CMAKE_CURRENT_LIST_DIR}/src/ui.c
		${CMAKE_CURRENT_LIST_DIR}/src/context.c

		${CMAKE_CURRENT_LIST_DIR}/include/game/detail.h
		${CMAKE_CURRENT_LIST_DIR}/include/game/result.h
		${CMAKE_CURRENT_LIST_DIR}/include/game/log.h
		${CMAKE_CURRENT_LIST_DIR}/include/game/display.h
		${CMAKE_CURRENT_LIST_DIR}/include/game/settings.h
		${CMAKE_CURRENT_LIST_DIR}/include/game/ui.h
		${CMAKE_CURRENT_LIST_DIR}/include/game/context.h)
