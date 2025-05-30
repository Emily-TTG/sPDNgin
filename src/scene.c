#include <game/scene.h>
#include <game/detail.h>
#include <game/components.h>

ECS_COMPONENT_DECLARE(gm_component_transform_t);
ECS_COMPONENT_DECLARE(gm_component_sprite_t);

enum gm_result gm_scene_new(struct gm_scene* scene, int argc, char** argv) {
	scene->world = ecs_init_w_args(argc, argv);

	ECS_COMPONENT_DEFINE(scene->world, gm_component_transform_t);
	ECS_COMPONENT_DEFINE(scene->world, gm_component_sprite_t);

	return GM_RESULT_OK;
}

void gm_scene_delete(struct gm_scene* scene) {
	ecs_fini(scene->world);
}
