#include <game/context.h>
#include <game/detail.h>
#include <game/log.h>
#include <game/settings.h>
#include <game/display.h>

static enum gm_result gm_context_install_events(struct gm_context* context) {
	auto detail = context->detail;

	ALLEGRO_TIMER* timer = al_create_timer(GM_FIXED_UPDATE_STEP);
	if(!timer) {
		gm_context_delete(context);
		return GM_LOG_RESULT(al_create_timer, GM_RESULT_ERROR);
	}
	detail->game_timer = timer;

	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
	if(!queue) {
		gm_context_delete(context);
		return GM_LOG_RESULT(al_create_timer, GM_RESULT_ERROR);
	}
	detail->event_queue = queue;

	if(!al_install_keyboard()) {
		gm_context_delete(context);
		return GM_LOG_RESULT(al_install_keyboard, GM_RESULT_ERROR);
	}

	if(!al_install_joystick()) {
		(void) GM_LOG_RESULT(al_install_joystick, GM_RESULT_ERROR);
		// TODO: Inform user that joystick support is disabled.
	}

	ALLEGRO_EVENT_SOURCE* source = al_get_keyboard_event_source();
	al_register_event_source(queue, source);

	source = al_get_joystick_event_source();
	if(source) {
		al_register_event_source(queue, source);
	}

	source = al_get_display_event_source(context->display->display);
	al_register_event_source(queue, source);

	source = al_get_timer_event_source(timer);
	al_register_event_source(queue, source);

	al_start_timer(timer);

	return GM_RESULT_OK;
}

enum gm_result gm_context_new(
		struct gm_context* context, int argc, char** argv) {

	enum gm_result result;

	// Needs to be early to retrieve information about display modes for
	// default settings.
	if(!al_init()) {
		return GM_LOG_RESULT(al_init, GM_RESULT_ERROR);
	}

	// TODO: Once/if this needs cleanup add to tail.
	result = gm_settings_load(context->settings, argc, argv);
	if(result) {
		return GM_LOG_RESULT(gm_settings_load, result);
	}

	result = gm_display_new(context, context->display);
	if(result) {
		return GM_LOG_RESULT(gm_settings_load, result);
	}

	result = gm_context_install_events(context);
	if(result) {
		return GM_LOG_RESULT(gm_settings_load, result);
	}

	return GM_RESULT_OK;
}

void gm_context_delete(struct gm_context* context) {
	auto detail = context->detail;

	al_destroy_timer(detail->game_timer);
	al_destroy_event_queue(detail->event_queue);

	gm_display_delete(context->display);

	// TODO: Can we meaningfully teardown Allegro global state?
}

enum gm_result gm_context_loop(struct gm_context* context) {
	auto detail = context->detail;

	ALLEGRO_EVENT event;
	while(true) {
		al_wait_for_event(detail->event_queue, &event);

		switch(event.type) {
			default: break;

			case ALLEGRO_EVENT_DISPLAY_CLOSE: goto end_loop;
		}
	}

	end_loop:;

	return GM_RESULT_OK;
}
