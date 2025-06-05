#pragma once

#include <pdn/result.h>

struct pdn_scene;

enum pdn_result pdn_scene_new(struct pdn_scene*, int, char**);
void pdn_scene_delete(struct pdn_scene*);
