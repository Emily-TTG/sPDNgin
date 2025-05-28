// Context data which requires Allegro to be included.

#pragma once

#include <allegro5/allegro5.h>

struct gm_context_detail {
	ALLEGRO_EVENT_QUEUE* event_queue;

	// Fixed rate game update timer.
	ALLEGRO_TIMER* game_timer;
};

struct gm_display {
	ALLEGRO_DISPLAY* display;
};
