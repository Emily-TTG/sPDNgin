// Does not assume context display is the target.

#pragma once

#include <pdn/result.h>

struct pdn_context;

// Provided by detail.
struct pdn_display;

enum pdn_result pdn_display_new(struct pdn_display*, struct pdn_context*);
void pdn_display_delete(struct pdn_display*);
