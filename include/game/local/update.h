#pragma once

#include <game/result.h>

struct gm_context;

enum gm_result gm_local_tick(struct gm_context*);
enum gm_result gm_local_render(struct gm_context*);
