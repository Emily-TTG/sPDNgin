#pragma once

#include <game/result.h>

struct gm_script;

typedef enum gm_result gm_script_table_handler_t(
		struct gm_script*, const char*, void*);

enum gm_result gm_script_new(struct gm_script*, const char*, bool);
void gm_script_delete(struct gm_script*);

int gm_script_table_get_int(struct gm_script*, int, const char*);

// Returns a heap allocated copy of the string.
char* gm_script_table_get_string(struct gm_script*, int, const char*);

enum gm_result gm_script_file_table(
		const char*, gm_script_table_handler_t*, void*);
