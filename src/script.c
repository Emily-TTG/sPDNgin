#include <pdn/script.h>
#include <pdn/detail.h>
#include <pdn/log.h>

enum pdn_result pdn_script_new(
		struct pdn_script* script, const char* path, bool do_libs) {

	script->state = luaL_newstate();

	if(do_libs) {
		luaL_openlibs(script->state);
	}

	int error = luaL_dofile(script->state, path);
	if(error) {
		PDN_LOG("%s: %s", path, lua_tostring(script->state, -1));
		lua_pop(script->state, 1);

		pdn_script_delete(script);

		return PDN_LOG_RESULT_PATH(luaL_dofile, path, PDN_RESULT_ERROR);
	}

	return PDN_RESULT_OK;
}

void pdn_script_delete(struct pdn_script* script) {
	lua_close(script->state);
}

int pdn_script_table_get_int(
		struct pdn_script* script, int index, const char* key) {

	int result;

	lua_getfield(script->state, index, key);
	{
		result = (int) lua_tointeger(script->state, -1);
	}
	lua_pop(script->state, 1);

	return result;
}

char* pdn_script_table_get_string(
		struct pdn_script* script, int index, const char* key) {

	lua_getfield(script->state, index, key);

	char* result = malloc(lua_strlen(script->state, -1) + 1);
	strcpy(result, lua_tostring(script->state, -1));
	lua_pop(script->state, 1);

	return result;
}

enum pdn_result pdn_script_file_table(
		const char* path, pdn_script_table_handler_t* handler, void* pass) {

	enum pdn_result result;

	struct pdn_script script;

	result = pdn_script_new(&script, path, false);
	if(result) {
		return PDN_LOG_RESULT_PATH(pdn_script_new, path, result);
	}

	if(!lua_istable(script.state, -1)) {
		pdn_script_delete(&script);
		return PDN_LOG_RESULT_PATH(lua_istable, path, PDN_RESULT_BAD_TYPE);
	}

	result = handler(&script, path, pass);
	if(result) {
		pdn_script_delete(&script);
		return PDN_LOG_RESULT_PATH(pdn_script_table_handler_t, path, result);
	}

	pdn_script_delete(&script);

	return PDN_RESULT_OK;
}
