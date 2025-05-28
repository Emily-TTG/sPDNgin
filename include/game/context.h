#pragma once

#include <game/result.h>

#define GM_FIXED_UPDATE_STEP (1.0 / 30.0)

struct gm_context_detail;

struct gm_settings;
struct gm_display;

struct gm_context {
	struct gm_context_detail* detail;

	struct gm_settings* settings;
	struct gm_display* display;
};

enum gm_result gm_context_new(struct gm_context*, int, char**);
void gm_context_delete(struct gm_context*);
