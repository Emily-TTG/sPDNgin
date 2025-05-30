#pragma once

#include <game/result.h>

struct gm_scene;

enum gm_result gm_scene_new(struct gm_scene*, int, char**);
void gm_scene_delete(struct gm_scene*);
