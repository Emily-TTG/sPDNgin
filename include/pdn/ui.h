#pragma once

#include <pdn/result.h>

struct pdn_context;

// Provided by detail.
struct pdn_ui;

enum pdn_result pdn_ui_new(
		struct pdn_ui*, struct pdn_context*, const char*, const char*);

void pdn_ui_delete(struct pdn_ui*);
