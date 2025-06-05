// Declares flecs components from our component types.

#pragma once

#include <pdn/detail.h>

// NOTE: Flecs component declarations need a single "word" for the ID so these
//		 Need to be typedef'd.

typedef struct pdn_component_transform {
	float x, y;
} pdn_component_transform_t;

typedef struct pdn_component_sprite {
	ALLEGRO_BITMAP* bitmap;
} pdn_component_sprite_t;

typedef struct pdn_component_tilemap {
	struct pdn_tilemap tilemap;
} pdn_component_tilemap_t;

extern ECS_COMPONENT_DECLARE(pdn_component_transform_t);
extern ECS_COMPONENT_DECLARE(pdn_component_sprite_t);
extern ECS_COMPONENT_DECLARE(pdn_component_tilemap_t);
