#pragma once

#include <pdn/result.h>

struct pdn_context_detail;

struct pdn_settings;
struct pdn_display;
struct pdn_ui;
struct pdn_scene;

struct ecs_world_t;

struct pdn_context {
	struct pdn_context_detail* detail;

	struct pdn_settings* settings;
	struct pdn_display* display;
	struct pdn_ui* ui;
	struct pdn_scene* scene;
};

enum pdn_result pdn_context_new(struct pdn_context*, int, char**);
void pdn_context_delete(struct pdn_context*);

// Blocks until user-initiated exit.
enum pdn_result pdn_context_loop(struct pdn_context*);
