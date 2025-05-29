#pragma once

#include <game/result.h>

struct gm_context;

// Provided by detail.
struct gm_ui;

enum gm_result gm_ui_new(struct gm_ui*, struct gm_context*);
void gm_ui_delete(struct gm_ui*);
