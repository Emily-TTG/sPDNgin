#include <game/result.h>
#include <game/log.h>
#include <game/context.h>

#include <allegro5/allegro5.h>

static enum gm_result gm_main() {
	struct gm_context context = {};
	(void) context;

	if(!al_init()) {
		return GM_LOG_RESULT(al_init, GM_RESULT_ERROR);
	}

	if(!al_install_keyboard()) {
		return GM_LOG_RESULT(al_install_keyboard, GM_RESULT_ERROR);
	}

	return GM_RESULT_OK;
}

int main(int argc, char** argv) {
	(void) argc;
	(void) argv;

	enum gm_result result = gm_main();

	if(result) {
		GM_LOG_RESULT("gm_main", result);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
