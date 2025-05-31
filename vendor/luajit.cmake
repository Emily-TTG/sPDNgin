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
	set(LUAJIT_BUILD_MODE "mixed")
elseif(${LUAJIT_STATIC})
	set(LUAJIT_BUILD_MODE "static")
elseif(${LUAJIT_SHARED})
	set(LUAJIT_BUILD_MODE "dynamic")
endif()

# TODO: As `MINGW` has no equivalent generator expression this needs to be
#       Configure-time determined.
#set(LUAJIT_CYGWIN_MSYS $<$<PLATFORM_ID:CYGWIN>,cygwin> $<$<PLATFORM_ID:MSYS>:mingw>)
if(CYGWIN)
	set(LUAJIT_HOST_MSYS "cygwin")
elseif(MSYS)
	set(LUAJIT_HOST_MSYS "mingw")
elseif(MINGW)
	set(LUAJIT_HOST_MSYS "mingw")
else()
	set(LUAJIT_HOST_MSYS)
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
		HOST_MSYS=${LUAJIT_HOST_MSYS}

		CC=${CMAKE_C_COMPILER}
		CFLAGS=${CMAKE_C_FLAGS}
		LDFLAGS=${LUAJIT_LDFLAGS}

		${LUAJIT_INSTALL_DIR_OPTION})

if(${CMAKE_GENERATOR} MATCHES "Unix Makefiles")
	set(LUAJIT_MAKE_PROGRAM ${CMAKE_MAKE_PROGRAM})
elseif(NOT LUAJIT_MAKE_PROGRAM)
	if(MINGW)
		set(LUAJIT_MAKE_PROGRAM mingw32-make)
	else()
		set(LUAJIT_MAKE_PROGRAM make)
	endif()
endif()

ExternalProject_Add(
		luajit-build

		SOURCE_DIR ${LUAJIT_SOURCE_DIR}
		BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}
		INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}

		CONFIGURE_COMMAND ""
		BUILD_COMMAND
			${LUAJIT_MAKE_PROGRAM} -C ${LUAJIT_SOURCE_DIR} ${LUAJIT_MAKE_ARGUMENTS}
		INSTALL_COMMAND
			${LUAJIT_MAKE_PROGRAM} -C ${LUAJIT_SOURCE_DIR} install ${LUAJIT_INSTALL_DIR_OPTION})

add_library(luajit INTERFACE)

target_link_libraries(luajit INTERFACE luajit-5.1)
target_include_directories(luajit INTERFACE ${LUAJIT_INSTALL_DIR}/include)
