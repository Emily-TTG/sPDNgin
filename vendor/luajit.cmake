# NOTE: Matching of generator/toolset/platform is not going to be perfect
#       But we'll try our best.

set(LUAJIT_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/luajit)

# TODO: Does this override a desirable user setting?
if(${BUILD_SHARED_LIBS})
	set(LUAJIT_SHARED ON)
else()
	set(LUAJIT_STATIC ON)
endif()

if(NOT LUAJIT_SHARED)
	set(LUAJIT_SHARED OFF)
endif()

if(${LUAJIT_STATIC} AND ${LUAJIT_SHARED})
	set(LUAJIT_BUILD_MODE mixed)
elseif(${LUAJIT_STATIC})
	set(LUAJIT_BUILD_MODE static)
elseif(${LUAJIT_SHARED})
	set(LUAJIT_BUILD_MODE dynamic)
endif()

set(
		LUAJIT_LDFLAGS
		${CMAKE_SHARED_LINKER_FLAGS}
		$<$<CONFIG:Debug>:${CMAKE_SHARED_LINKER_FLAGS_DEBUG}>
		$<$<CONFIG:Release>:${CMAKE_SHARED_LINKER_FLAGS_RELEASE}>)

set(LUAJIT_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/luajit)
set(LUAJIT_INSTALL_DIR_OPTION PREFIX=${LUAJIT_INSTALL_DIR})

set(
		LUAJIT_MAKE_ARGUMENTS

		BUILDMODE=${LUAJIT_BUILD_MODE}
		CCDEBUG=$<$<CONFIG:Debug>:${CMAKE_C_FLAGS_DEBUG}>
		CCOPT=$<$<CONFIG:Release>:${CMAKE_C_FLAGS_RELEASE}>

		HOST_SYS=$<$<PLATFORM_ID:Windows>:Windows>

		CC=${CMAKE_C_COMPILER}
		#CFLAGS=${CMAKE_C_FLAGS}
		#LDFLAGS=${LUAJIT_LDFLAGS}

		${LUAJIT_INSTALL_DIR_OPTION})

if(${CMAKE_GENERATOR} MATCHES "Unix Makefiles")
	set(LUAJIT_MAKE_PROGRAM ${CMAKE_MAKE_PROGRAM})
elseif(NOT LUAJIT_MAKE_PROGRAM)
	if(MINGW)
		# TODO: This doesn't work in a cross context.
		set(LUAJIT_MAKE_PROGRAM mingw32-make)
	else()
		set(LUAJIT_MAKE_PROGRAM make)
	endif()
endif()

set(LUAJIT_MAKE_DIR ${LUAJIT_SOURCE_DIR}/$<$<PLATFORM_ID:Windows>:src>)

ExternalProject_Add(
		luajit-build

		SOURCE_DIR ${LUAJIT_SOURCE_DIR}

		CONFIGURE_COMMAND ""
		BUILD_COMMAND ${CMAKE_COMMAND} -E env ${LUAJIT_MAKE_PROGRAM} -C ${LUAJIT_MAKE_DIR} ${LUAJIT_MAKE_ARGUMENTS}
		INSTALL_COMMAND "")

add_library(luajit-lib INTERFACE)
add_dependencies(luajit-lib luajit-build)

target_include_directories(luajit-lib INTERFACE ${LUAJIT_SOURCE_DIR}/src)

target_link_directories(luajit-lib INTERFACE ${LUAJIT_SOURCE_DIR}/src)
target_link_libraries(luajit-lib INTERFACE luajit)
