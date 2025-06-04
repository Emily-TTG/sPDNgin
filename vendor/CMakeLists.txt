set(BUILD_SHARED_LIBS OFF)

set(WANT_STATIC_RUNTIME ON)
set(PREFER_STATIC_DEPS ON)
set(SHARED OFF)
set(WANT_DOCS OFF)
set(WANT_DEMO OFF)
set(WANT_EXAMPLES OFF)
set(WANT_TESTS OFF)
set(WANT_MONOLITH ON)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/allegro SYSTEM)

# Windows+Submodule needs some fanagling.
add_library(allegro_broken_includes INTERFACE)
get_target_property(AL_FORCE_INCLUDE allegro_monolith INCLUDE_DIRECTORIES)
target_include_directories(allegro_broken_includes INTERFACE ${AL_FORCE_INCLUDE})

set(FLECS_SHARED OFF)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/flecs SYSTEM)

include(${CMAKE_CURRENT_LIST_DIR}/nuklear.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/stb.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/luajit.cmake)
