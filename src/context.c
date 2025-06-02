#include <game/context.h>
#include <game/detail.h>
#include <game/log.h>
#include <game/settings.h>
#include <game/display.h>
#include <game/ui.h>
#include <game/scene.h>
#include <game/script.h>

#include <game/local/local.h>

static enum gm_result gm_context_install_events(struct gm_context* context) {
	auto detail = context->detail;

	// TODO: Handle sync rules. If uncapped -- this doesn't need to be a timer
	//		 And the update loop should run unconstrained (i.e. poll rather
	//		 Than block for events).
	ALLEGRO_TIMER* timer = al_create_timer(1.0 / context->settings->framerate);
	if(!timer) {
		return GM_LOG_RESULT(al_create_timer, GM_RESULT_ERROR);
	}
	detail->timer = timer;

	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
	if(!queue) {
		return GM_LOG_RESULT(al_create_timer, GM_RESULT_ERROR);
	}
	detail->event_queue = queue;

	if(!al_install_keyboard()) {
		return GM_LOG_RESULT(al_install_keyboard, GM_RESULT_ERROR);
	}

	if(!al_install_mouse()) {
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

	al_start_timer(detail->timer);

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

	result = gm_settings_load(context->settings, argc, argv);
	if(result) {
		gm_context_delete(context);
		return GM_LOG_RESULT(gm_settings_load, result);
	}

	// TODO: App config for window title etc.
	result = gm_display_new(context->display, context);
	if(result) {
		gm_context_delete(context);
		return GM_LOG_RESULT(gm_settings_load, result);
	}

	result = gm_context_install_events(context);
	if(result) {
		gm_context_delete(context);
		return GM_LOG_RESULT(gm_settings_load, result);
	}

	result = gm_ui_new(context->ui, context);
	if(result) {
		gm_context_delete(context);
		return GM_LOG_RESULT(gm_ui_new, result);
	}

	result = gm_scene_new(context->scene, argc, argv);
	if(result) {
		gm_context_delete(context);
		return GM_LOG_RESULT(gm_scene_new, result);
	}

	result = gm_script_engine_new(context->engine);
	if(result) {
		gm_context_delete(context);
		return GM_LOG_RESULT(gm_scene_new, result);
	}

	return GM_RESULT_OK;
}

void gm_context_delete(struct gm_context* context) {
	auto detail = context->detail;

	al_uninstall_keyboard();
	al_uninstall_mouse();
	al_uninstall_joystick();

	al_destroy_timer(detail->timer);
	al_destroy_event_queue(detail->event_queue);

	gm_script_engine_delete(context->engine);
	gm_scene_delete(context->scene);
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

	bool tick = false;
	// TODO: Once note on timer init is resolved, also resolve this.
	double delta = 1.0 / context->settings->framerate;

	bool exit = false;

	result = gm_local_start(context);
	if(result) {
		return GM_LOG_RESULT(gm_local_start, result);
	}

	while(!exit) {
		al_wait_for_event(detail->event_queue, &event);

		nk_input_begin(ui->context);
		do {
			nk_allegro5_handle_event(&event);

			if(event.type == ALLEGRO_EVENT_TIMER) {
				auto timer = event.timer;

				tick = (timer.source == context->detail->timer);
			}
			else gm_context_handle_event(context, &event, &exit);
		} while(al_get_next_event(detail->event_queue, &event));
		nk_input_end(ui->context);

		if(tick) {
			result = gm_local_update(context, delta);
			if(result) {
				(void) GM_LOG_RESULT(gm_local_update, result);
			}

			nk_allegro5_render();
			// TODO: Sync rules?
			al_flip_display();
		}
	}

	return GM_RESULT_OK;
}
