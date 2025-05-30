#pragma once

#include <game/result.h>

struct gm_context;

enum gm_result gm_local_start(struct gm_context*);
enum gm_result gm_local_update(struct gm_context*, double);
