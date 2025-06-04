#include <game/script.h>
#include <game/detail.h>
#include <game/log.h>

enum gm_result gm_script_new(
		struct gm_script* script, const char* path, bool do_libs) {

	script->state = luaL_newstate();

	if(do_libs) {
		luaL_openlibs(script->state);
	}

	int error = luaL_dofile(script->state, path);
	if(error) {
		GM_LOG("%s: %s", path, lua_tostring(script->state, -1));
		lua_pop(script->state, 1);

		gm_script_delete(script);

		return GM_LOG_RESULT_PATH(luaL_dofile, path, GM_RESULT_ERROR);
	}

	return GM_RESULT_OK;
}

void gm_script_delete(struct gm_script* script) {
	lua_close(script->state);
}

int gm_script_table_get_int(
		struct gm_script* script, int index, const char* key) {

	int result;

	lua_getfield(script->state, index, key);
	{
		result = (int) lua_tointeger(script->state, -1);
	}
	lua_pop(script->state, 1);

	return result;
}

char* gm_script_table_get_string(
		struct gm_script* script, int index, const char* key) {

	lua_getfield(script->state, index, key);

	char* result = malloc(lua_strlen(script->state, -1) + 1);
	strcpy(result, lua_tostring(script->state, -1));
	lua_pop(script->state, 1);

	return result;
}

enum gm_result gm_script_file_table(
		const char* path, gm_script_table_handler_t* handler, void* pass) {

	enum gm_result result;

	struct gm_script script;

	result = gm_script_new(&script, path, false);
	if(result) {
		return GM_LOG_RESULT_PATH(gm_script_new, path, result);
	}

	if(!lua_istable(script.state, -1)) {
		gm_script_delete(&script);
		return GM_LOG_RESULT_PATH(lua_istable, path, GM_RESULT_BAD_TYPE);
	}

	result = handler(&script, path, pass);
	if(result) {
		gm_script_delete(&script);
		return GM_LOG_RESULT_PATH(gm_script_table_handler_t, path, result);
	}

	gm_script_delete(&script);

	return GM_RESULT_OK;
}
