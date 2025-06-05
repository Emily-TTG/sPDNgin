#pragma once

#include <pdn/result.h>

struct pdn_script;

typedef enum pdn_result pdn_script_table_handler_t(
		struct pdn_script*, const char*, void*);

enum pdn_result pdn_script_new(struct pdn_script*, const char*, bool);
void pdn_script_delete(struct pdn_script*);

int pdn_script_table_get_int(struct pdn_script*, int, const char*);

// Returns a heap allocated copy of the string.
char* pdn_script_table_get_string(struct pdn_script*, int, const char*);

enum pdn_result pdn_script_file_table(
		const char*, pdn_script_table_handler_t*, void*);
