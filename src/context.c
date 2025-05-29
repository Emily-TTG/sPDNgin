#include <game/context.h>
#include <game/detail.h>
#include <game/log.h>
#include <game/settings.h>
#include <game/display.h>
#include <game/ui.h>

#include <game/local/local.h>

static enum gm_result gm_context_install_events(struct gm_context* context) {
	auto detail = context->detail;

	ALLEGRO_TIMER* timer = al_create_timer(GM_FIXED_UPDATE_STEP);
	if(!timer) {
		gm_context_delete(context);
		return GM_LOG_RESULT(al_create_timer, GM_RESULT_ERROR);
	}
	detail->game_timer = timer;

	// TODO: Handle sync rules.
	timer = al_create_timer(1.0 / (double) context->settings->framerate);
	if(!timer) {
		gm_context_delete(context);
		return GM_LOG_RESULT(al_create_timer, GM_RESULT_ERROR);
	}
	detail->render_timer = timer;

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

	if(!al_install_mouse()) {
		gm_context_delete(context);
		return GM_LOG_RESULT(al_install_mouse, GM_RESULT_ERROR);
	}

	if(!al_install_joystick()) {
		(void) GM_LOG_RESULT(al_install_joystick, GM_RESULT_ERROR);
		// TODO: Inform user that joystick support is disabled.
	}

	ALLEGRO_EVENT_SOURCE* source = al_get_keyboard_event_source();
	al_register_event_source(queue, source);

	source = al_get_mouse_event_source();
	al_register_event_source(queue, source);

	source = al_get_joystick_event_source();
	if(source) {
		al_register_event_source(queue, source);
	}

	source = al_get_display_event_source(context->display->display);
	al_register_event_source(queue, source);

	source = al_get_timer_event_source(timer);
	al_register_event_source(queue, source);

	al_start_timer(detail->game_timer);
	al_start_timer(detail->render_timer);

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
	// TODO: Leaky EH in here.
	result = gm_settings_load(context->settings, argc, argv);
	if(result) {
		return GM_LOG_RESULT(gm_settings_load, result);
	}

	// TODO: App config for window title etc.
	result = gm_display_new(context->display, context);
	if(result) {
		return GM_LOG_RESULT(gm_settings_load, result);
	}

	result = gm_context_install_events(context);
	if(result) {
		return GM_LOG_RESULT(gm_settings_load, result);
	}

	result = gm_ui_new(context->ui, context);
	if(result) {
		return GM_LOG_RESULT(gm_ui_new, result);
	}

	return GM_RESULT_OK;
}

void gm_context_delete(struct gm_context* context) {
	auto detail = context->detail;

	al_uninstall_keyboard();
	al_uninstall_mouse();
	al_uninstall_joystick();

	al_destroy_timer(detail->game_timer);
	al_destroy_timer(detail->render_timer);
	al_destroy_event_queue(detail->event_queue);

	gm_ui_delete(context->ui);
	gm_display_delete(context->display);

	// TODO: Can we meaningfully teardown Allegro global state?
}

static enum gm_result gm_context_handle_event(
		struct gm_context* context, ALLEGRO_EVENT* event, bool* exit) {

	(void) context;

	switch(event->type) {
		default: break;

		case ALLEGRO_EVENT_DISPLAY_CLOSE: {
			*exit = true;
			break;
		}
	}

	return GM_RESULT_OK;
}

enum gm_result gm_context_loop(struct gm_context* context) {
	enum gm_result result;

	auto detail = context->detail;
	auto ui = context->ui;

	ALLEGRO_EVENT event;

	bool tick_game = false;
	bool tick_render = false;
	bool exit = false;

	result = gm_local_start_game(context);
	if(result) {
		return GM_LOG_RESULT(gm_local_start_game, result);
	}

	result = gm_local_start_render(context);
	if(result) {
		return GM_LOG_RESULT(gm_local_start_render, result);
	}

	while(!exit) {
		al_wait_for_event(detail->event_queue, &event);

		nk_input_begin(ui->context);
		do {
			nk_allegro5_handle_event(&event);

			if(event.type == ALLEGRO_EVENT_TIMER) {
				auto timer = event.timer;

				tick_game = (timer.source == context->detail->game_timer);
				tick_render = (timer.source == context->detail->render_timer);
			}
			else gm_context_handle_event(context, &event, &exit);
		} while(al_get_next_event(detail->event_queue, &event));
		nk_input_end(ui->context);


		if(tick_game) {
			result = gm_local_tick_game(context);
			if(result) {
				(void) GM_LOG_RESULT(gm_local_tick_game, result);
			}
		}

		if(tick_render) {
			result = gm_local_tick_render(context);
			if(result) {
				(void) GM_LOG_RESULT(gm_local_tick_render, result);
			}

			nk_allegro5_render();
			// TODO: Sync rules?
			al_flip_display();
		}
	}

	return GM_RESULT_OK;
}
