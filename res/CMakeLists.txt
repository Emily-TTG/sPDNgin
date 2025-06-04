function(tiled_generate _INPUT _OUTPUT _FORMAT _TYPE _LIST)
	set(_LOCAL_OUTPUT ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/${_OUTPUT})
	set(_LOCAL_INPUT ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/${_INPUT})

	add_custom_command(
			OUTPUT ${_LOCAL_OUTPUT}
			COMMAND tiled
				ARGS --export-${_TYPE} ${_FORMAT} ${_LOCAL_INPUT} ${_LOCAL_OUTPUT}
			MAIN_DEPENDENCY ${_LOCAL_INPUT})

	set(${_LIST} ${${_LIST}} ${_LOCAL_OUTPUT} PARENT_SCOPE)
endfunction()

tiled_generate(map/src/test.tmx map/test.lua lua map ASSET_TARGETS)
tiled_generate(tile/src/goop.tsx tile/goop.lua lua tileset ASSET_TARGETS)

add_custom_target(assets DEPENDS ${ASSET_TARGETS})
