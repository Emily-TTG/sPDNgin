add_executable(configurator WIN32)

target_compile_features(configurator PRIVATE c_std_23)

cxx_default_diagnostics(configurator)

target_sources(configurator PRIVATE ${CMAKE_CURRENT_LIST_DIR}/configurator.c)
target_link_libraries(configurator PRIVATE nuklear-interface)
