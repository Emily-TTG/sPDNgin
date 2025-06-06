add_library(nuklear-interface INTERFACE)

target_sources(
		nuklear-interface INTERFACE
		${CMAKE_CURRENT_LIST_DIR}/nuklear/nuklear.h
		${CMAKE_CURRENT_LIST_DIR}/nuklear/demo/allegro5/nuklear_allegro5.h)

target_include_directories(
		nuklear-interface SYSTEM INTERFACE
		${CMAKE_CURRENT_LIST_DIR}/nuklear
		${CMAKE_CURRENT_LIST_DIR}/nuklear/demo/allegro5)

target_link_libraries(
		nuklear-interface INTERFACE
		allegro_broken_includes
		allegro_monolith
		${MATH_LIBRARY})

target_compile_definitions(
		nuklear-interface INTERFACE
		NK_INCLUDE_FIXED_TYPES=1
		NK_INCLUDE_DEFAULT_ALLOCATOR=1
		NK_INCLUDE_STANDARD_IO=1
		NK_INCLUDE_STANDARD_VARARGS=1
		# TODO: This is currently broken by
		#       https://github.com/Immediate-Mode-UI/Nuklear/issues/812
		#NK_INCLUDE_STANDARD_BOOL=1
		NK_INCLUDE_FONT_BAKING=1)

add_library(nuklear STATIC)

target_link_libraries(nuklear PUBLIC nuklear-interface)

target_sources(nuklear PRIVATE ${CMAKE_CURRENT_LIST_DIR}/helper/nuklear.c)
