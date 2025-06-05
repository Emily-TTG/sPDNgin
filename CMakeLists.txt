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

include(ExternalProject)

find_library(MATH_LIBRARY m)
if(NOT ${MATH_LIBRARY})
	set(MATH_LIBRARY "")
endif()

include(${CMAKE_CURRENT_LIST_DIR}/cmake/utilities.cmake)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/vendor)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/tool)

add_library(pdn STATIC)
target_compile_features(pdn PUBLIC c_std_23)

cxx_default_diagnostics(pdn)

target_include_directories(pdn PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
target_sources(
		pdn PRIVATE
		${CMAKE_CURRENT_LIST_DIR}/src/result.c
		${CMAKE_CURRENT_LIST_DIR}/src/log.c
		${CMAKE_CURRENT_LIST_DIR}/src/display.c
		${CMAKE_CURRENT_LIST_DIR}/src/settings.c
		${CMAKE_CURRENT_LIST_DIR}/src/ui.c
		${CMAKE_CURRENT_LIST_DIR}/src/scene.c
		${CMAKE_CURRENT_LIST_DIR}/src/script.c
		${CMAKE_CURRENT_LIST_DIR}/src/tilemap.c
		${CMAKE_CURRENT_LIST_DIR}/src/context.c)

target_sources(
		pdn PUBLIC
		${CMAKE_CURRENT_LIST_DIR}/include/pdn/detail.h
		${CMAKE_CURRENT_LIST_DIR}/include/pdn/result.h
		${CMAKE_CURRENT_LIST_DIR}/include/pdn/log.h
		${CMAKE_CURRENT_LIST_DIR}/include/pdn/display.h
		${CMAKE_CURRENT_LIST_DIR}/include/pdn/settings.h
		${CMAKE_CURRENT_LIST_DIR}/include/pdn/ui.h
		${CMAKE_CURRENT_LIST_DIR}/include/pdn/scene.h
		${CMAKE_CURRENT_LIST_DIR}/include/pdn/script.h
		${CMAKE_CURRENT_LIST_DIR}/include/pdn/components.h
		${CMAKE_CURRENT_LIST_DIR}/include/pdn/tilemap.h
		${CMAKE_CURRENT_LIST_DIR}/include/pdn/context.h
		${CMAKE_CURRENT_LIST_DIR}/include/pdn/local.h)

target_link_libraries(
		pdn PUBLIC
		stb_image
		allegro_broken_includes
		allegro_monolith
		nuklear
		flecs::flecs_static
		luajit-lib)
