// Declares flecs components from our component types.

#pragma once

#include <game/detail.h>

// NOTE: Flecs component declarations need a single "word" for the ID so these
//		 Need to be typedef'd.

typedef struct gm_component_transform {
	float x, y;
} gm_component_transform_t;

typedef struct gm_component_sprite {
	ALLEGRO_BITMAP* bitmap;
} gm_component_sprite_t;

extern ECS_COMPONENT_DECLARE(gm_component_transform_t);
extern ECS_COMPONENT_DECLARE(gm_component_sprite_t);
