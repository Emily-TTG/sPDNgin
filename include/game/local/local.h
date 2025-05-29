#pragma once

#include <game/result.h>

struct gm_context;

enum gm_result gm_local_start_game(struct gm_context*);
enum gm_result gm_local_tick_game(struct gm_context*);

enum gm_result gm_local_start_render(struct gm_context*);
enum gm_result gm_local_tick_render(struct gm_context*);
