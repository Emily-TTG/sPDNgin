add_library(stb_image STATIC)
target_include_directories(stb_image PUBLIC ${CMAKE_CURRENT_LIST_DIR}/stb SYSTEM)
target_sources(stb_image PRIVATE ${CMAKE_CURRENT_LIST_DIR}/helper/stb_image.c)
