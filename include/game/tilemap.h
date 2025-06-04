#pragma once

#include <game/result.h>

struct gm_script;

struct gm_tileset;
struct gm_tilemap;

enum gm_result gm_tileset_script_table_handler(
		struct gm_script*, const char*, void*);

enum gm_result gm_tilemap_script_table_handler(
		struct gm_script*, const char*, void*);
