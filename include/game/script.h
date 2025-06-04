#pragma once

#include <game/result.h>

struct gm_script_engine;

enum gm_result gm_script_engine_new(struct gm_script_engine*);
void gm_script_engine_delete(struct gm_script_engine*);
