set(BUILD_SHARED_LIBS OFF)

set(WANT_STATIC_RUNTIME ON)
set(PREFER_STATIC_DEPS ON)
set(SHARED OFF)
set(WANT_DOCS OFF)
set(WANT_DEMO OFF)
set(WANT_EXAMPLES OFF)
set(WANT_TESTS OFF)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/allegro SYSTEM)

add_library(nuklear STATIC)

target_sources(nuklear PRIVATE ${CMAKE_CURRENT_LIST_DIR}/helper/nuklear.c)
target_sources(
		nuklear PUBLIC
		${CMAKE_CURRENT_LIST_DIR}/nuklear/nuklear.h
		${CMAKE_CURRENT_LIST_DIR}/nuklear/demo/allegro5/nuklear_allegro5.h)

target_link_libraries(nuklear PRIVATE allegro)

target_include_directories(
		nuklear PUBLIC
		${CMAKE_CURRENT_LIST_DIR}/nuklear
		${CMAKE_CURRENT_LIST_DIR}/nuklear/demo/allegro5)

target_compile_definitions(
		nuklear PUBLIC
		NK_INCLUDE_FIXED_TYPES=1
		NK_INCLUDE_DEFAULT_ALLOCATOR=1
		NK_INCLUDE_STANDARD_IO=1
		NK_INCLUDE_STANDARD_VARARGS=1
		NK_INCLUDE_STANDARD_BOOL=1
		NK_INCLUDE_FONT_BAKING=1
		NK_INCLUDE_DEFAULT_FONT=1)
