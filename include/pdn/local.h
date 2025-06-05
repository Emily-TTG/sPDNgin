#pragma once

#include <pdn/result.h>

struct pdn_context;

enum pdn_result pdn_local_start(struct pdn_context*);
enum pdn_result pdn_local_update(struct pdn_context*, double);
