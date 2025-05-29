// Context data which requires vendored code to be included.

#pragma once

#include <allegro5/allegro5.h>

#include <nuklear.h>
#include <nuklear_allegro5.h>

struct gm_context_detail {
	ALLEGRO_EVENT_QUEUE* event_queue;

	ALLEGRO_TIMER* game_timer; // Fixed rate game update timer.
	ALLEGRO_TIMER* render_timer;
};

struct gm_display {
	ALLEGRO_DISPLAY* display;
};

struct gm_ui {
	NkAllegro5Font* font;

	struct nk_context* context;
};
