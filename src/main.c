#include <game/result.h>
#include <game/log.h>
#include <game/context.h>
#include <game/settings.h>
#include <game/detail.h>

static enum gm_result gm_main(int argc, char** argv) {
	enum gm_result result;

	struct gm_context_detail detail = {};

	struct gm_settings settings = {};
	struct gm_display display = {};

	struct gm_context context = {
			.detail = &detail,

			.settings = &settings,
			.display = &display
	};

	result = gm_context_new(&context, argc, argv);
	if(result) {
		return GM_LOG_RESULT("gm_context_new", result);
	}

	gm_context_delete(&context);

	return GM_RESULT_OK;
}

int main(int argc, char** argv) {
	enum gm_result result = gm_main(argc, argv);

	if(result) {
		(void) GM_LOG_RESULT("gm_main", result);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
