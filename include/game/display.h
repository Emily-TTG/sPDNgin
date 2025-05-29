// Does not assume context display is the target.

#pragma once

#include <game/result.h>

struct gm_context;

// Provided by detail.
struct gm_display;

enum gm_result gm_display_new(struct gm_display*, struct gm_context*);
void gm_display_delete(struct gm_display*);
