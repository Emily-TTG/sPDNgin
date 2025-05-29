#include <game/display.h>
#include <game/context.h>
#include <game/detail.h>
#include <game/settings.h>
#include <game/log.h>

enum gm_result gm_display_new(
		struct gm_display* display, struct gm_context* context) {

	auto settings = context->settings;

	ALLEGRO_DISPLAY* handle = al_create_display(
			(int) settings->width, (int) settings->height);

	if(!handle) {
		return GM_LOG_RESULT(al_create_display, GM_RESULT_ERROR);
	}

	display->display = handle;

	return GM_RESULT_OK;
}

void gm_display_delete(struct gm_display* display) {
	al_destroy_display(display->display);
}
