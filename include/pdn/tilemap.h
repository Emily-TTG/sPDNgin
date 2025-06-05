#pragma once

#include <pdn/result.h>

struct pdn_script;

struct pdn_tileset;
struct pdn_tilemap;

enum pdn_result pdn_tileset_script_table_handler(
		struct pdn_script*, const char*, void*);

enum pdn_result pdn_tilemap_script_table_handler(
		struct pdn_script*, const char*, void*);
