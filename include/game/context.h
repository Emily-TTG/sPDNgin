#pragma once

#include <game/result.h>

struct gm_context_detail;

struct gm_settings;
struct gm_display;
struct gm_ui;
struct gm_scene;

struct ecs_world_t;

struct gm_context {
	struct gm_context_detail* detail;

	struct gm_settings* settings;
	struct gm_display* display;
	struct gm_ui* ui;
	struct gm_scene* scene;
};

enum gm_result gm_context_new(struct gm_context*, int, char**);
void gm_context_delete(struct gm_context*);

// Blocks until user-initiated exit.
enum gm_result gm_context_loop(struct gm_context*);
