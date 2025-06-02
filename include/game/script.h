// NOTE: Script engine does not use detail for lua inclusion as the Lua
//		 Top-level API is exposed to consumers -- no need to rewrite a table
//		 Access/manipulation Interface ourselves.
#pragma once

#include <game/result.h>

#include <luajit-2.1/lua.h>
#include <luajit-2.1/lualib.h>
#include <luajit-2.1/lauxlib.h>

struct gm_script_engine {
	lua_State* state;
};

enum gm_result gm_script_engine_new(struct gm_script_engine*);
void gm_script_engine_delete(struct gm_script_engine*);
