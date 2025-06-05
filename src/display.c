#include <pdn/display.h>
#include <pdn/context.h>
#include <pdn/detail.h>
#include <pdn/settings.h>
#include <pdn/log.h>

enum pdn_result pdn_display_new(
		struct pdn_display* display, struct pdn_context* context) {

	auto settings = context->settings;

	ALLEGRO_DISPLAY* handle = al_create_display(
			(int) settings->width, (int) settings->height);

	if(!handle) {
		return PDN_LOG_RESULT(al_create_display, PDN_RESULT_ERROR);
	}

	display->display = handle;

	return PDN_RESULT_OK;
}

void pdn_display_delete(struct pdn_display* display) {
	al_destroy_display(display->display);
}
