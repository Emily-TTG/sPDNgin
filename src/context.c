#include <pdn/context.h>
#include <pdn/detail.h>
#include <pdn/log.h>
#include <pdn/settings.h>
#include <pdn/display.h>
#include <pdn/ui.h>
#include <pdn/scene.h>
#include <pdn/script.h>

#include <pdn/local.h>

static enum pdn_result pdn_context_install_events(struct pdn_context* context) {
	auto detail = context->detail;

	// TODO: Handle sync rules. If uncapped -- this doesn't need to be a timer
	//		 And the update loop should run unconstrained (i.e. poll rather
	//		 Than block for events).
	ALLEGRO_TIMER* timer = al_create_timer(1.0 / context->settings->framerate);
	if(!timer) {
		return PDN_LOG_RESULT(al_create_timer, PDN_RESULT_ERROR);
	}
	detail->timer = timer;

	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
	if(!queue) {
		return PDN_LOG_RESULT(al_create_timer, PDN_RESULT_ERROR);
	}
	detail->event_queue = queue;

	if(!al_install_keyboard()) {
		return PDN_LOG_RESULT(al_install_keyboard, PDN_RESULT_ERROR);
	}

	if(!al_install_mouse()) {
		return PDN_LOG_RESULT(al_install_mouse, PDN_RESULT_ERROR);
	}

	if(!al_install_joystick()) {
		(void) PDN_LOG_RESULT(al_install_joystick, PDN_RESULT_ERROR);
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

	return PDN_RESULT_OK;
}

static ALLEGRO_BITMAP* pdn_al_load_stbi_png(const char* path, int flags) {
	(void) flags;

	int width, height, channels;
	stbi_uc* data = stbi_load(path, &width, &height, &channels, 0);
	if(!data) {
		(void) PDN_LOG_RESULT_PATH(stbi_load, path, PDN_RESULT_ERROR);
		return 0;
	}

	ALLEGRO_BITMAP* bitmap = al_create_bitmap(width, height);
	if(!bitmap) {
		stbi_image_free(data);
		(void) PDN_LOG_RESULT(al_create_bitmap, PDN_RESULT_ERROR);
		return 0;
	}

	al_set_target_bitmap(bitmap);

	for(int y = 0; y < height; ++y) {
		for(int x = 0; x < width; ++x) {
			ALLEGRO_COLOR color;
			stbi_uc* base = &data[(x + y * width) * channels];

			if(channels == 3) {
				 color = al_map_rgb(base[0], base[1], base[2]);
			}
			else if(channels == 4) {
				color = al_map_rgba(base[0], base[1], base[2], base[3]);
			}
			else {
				stbi_image_free(data);
				PDN_LOG("Cannot convert image %s: channels != 3/4", path);
				return 0;
			}

			al_draw_pixel((float) x, (float) y, color);
		}
	}
	stbi_image_free(data);

	return bitmap;
}

enum pdn_result pdn_context_new(
		struct pdn_context* context, struct pdn_context_options* options,
		int argc, char** argv) {

	enum pdn_result result;

	// Needs to be early to retrieve information about display modes for
	// default settings.
	if(!al_init()) {
		return PDN_LOG_RESULT(al_init, PDN_RESULT_ERROR);
	}

	bool set = al_register_bitmap_loader(".png", pdn_al_load_stbi_png);
	if(!set) {
		(void) PDN_LOG_RESULT(al_register_bitmap_loader, PDN_RESULT_ERROR);
	}

	result = pdn_settings_load(context->settings, argc, argv);
	if(result) {
		pdn_context_delete(context);
		return PDN_LOG_RESULT(pdn_settings_load, result);
	}

	// TODO: App config for window title etc.
	result = pdn_display_new(context->display, context);
	if(result) {
		pdn_context_delete(context);
		return PDN_LOG_RESULT(pdn_settings_load, result);
	}

	result = pdn_context_install_events(context);
	if(result) {
		pdn_context_delete(context);
		return PDN_LOG_RESULT(pdn_settings_load, result);
	}

	result = pdn_ui_new(context->ui, context, options->font, options->style);
	if(result) {
		pdn_context_delete(context);
		return PDN_LOG_RESULT(pdn_ui_new, result);
	}

	result = pdn_scene_new(context->scene, argc, argv);
	if(result) {
		pdn_context_delete(context);
		return PDN_LOG_RESULT(pdn_scene_new, result);
	}

	return PDN_RESULT_OK;
}

void pdn_context_delete(struct pdn_context* context) {
	auto detail = context->detail;

	al_uninstall_keyboard();
	al_uninstall_mouse();
	al_uninstall_joystick();

	al_destroy_timer(detail->timer);
	al_destroy_event_queue(detail->event_queue);

	pdn_scene_delete(context->scene);
	pdn_ui_delete(context->ui);
	pdn_display_delete(context->display);

	// TODO: Can we meaningfully teardown Allegro global state?
}

static enum pdn_result pdn_context_handle_event(
		struct pdn_context* context, ALLEGRO_EVENT* event, bool* exit) {

	(void) context;

	switch(event->type) {
		default: break;

		case ALLEGRO_EVENT_DISPLAY_CLOSE: {
			*exit = true;
			break;
		}
	}

	return PDN_RESULT_OK;
}

enum pdn_result pdn_context_loop(struct pdn_context* context) {
	enum pdn_result result;

	auto detail = context->detail;
	auto ui = context->ui;

	ALLEGRO_EVENT event;

	bool tick = false;
	// TODO: Once note on timer init is resolved, also resolve this.
	double delta = 1.0 / context->settings->framerate;

	bool exit = false;

	result = pdn_local_start(context);
	if(result) {
		return PDN_LOG_RESULT(pdn_local_start, result);
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
			else pdn_context_handle_event(context, &event, &exit);
		} while(al_get_next_event(detail->event_queue, &event));
		nk_input_end(ui->context);

		if(tick) {
			result = pdn_local_update(context, delta);
			if(result) {
				(void) PDN_LOG_RESULT(pdn_local_update, result);
			}

			nk_allegro5_render();
			// TODO: Sync rules?
			al_flip_display();
		}
	}

	return PDN_RESULT_OK;
}
