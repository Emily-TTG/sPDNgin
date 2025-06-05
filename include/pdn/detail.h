// Context data which requires vendored code to be included.

#pragma once

#include <allegro5/allegro.h>

#include <nuklear.h>
#include <nuklear_allegro5.h>

#include <flecs.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <stb_image.h>

struct pdn_context_detail {
	ALLEGRO_EVENT_QUEUE* event_queue;

	ALLEGRO_TIMER* timer;
};

struct pdn_display {
	ALLEGRO_DISPLAY* display;
};

struct pdn_ui {
	NkAllegro5Font* font;

	struct nk_context* context;
};

struct pdn_scene {
	struct ecs_world_t* world;
};

struct pdn_script {
	lua_State* state;
};

struct pdn_tileset {
	ALLEGRO_BITMAP* atlas;

	int dimension;
	int width, height;
};

struct pdn_tilemap {
	struct pdn_tileset tileset;

	int width, height, layers;
	int** data;
};
