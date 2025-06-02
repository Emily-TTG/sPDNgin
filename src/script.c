#include <game/script.h>
#include <game/detail.h>

enum gm_result gm_script_engine_new(struct gm_script_engine* engine) {
	engine->state = luaL_newstate();

	luaL_openlibs(engine->state);

	al_fgets()

	return GM_RESULT_OK;
}

void gm_script_engine_delete(struct gm_script_engine* engine) {
	lua_close(engine->state);
}
