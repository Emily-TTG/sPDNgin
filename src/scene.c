#include <pdn/scene.h>
#include <pdn/detail.h>
#include <pdn/components.h>

ECS_COMPONENT_DECLARE(pdn_component_transform_t);
ECS_COMPONENT_DECLARE(pdn_component_sprite_t);
ECS_COMPONENT_DECLARE(pdn_component_tilemap_t);

enum pdn_result pdn_scene_new(struct pdn_scene* scene, int argc, char** argv) {
	scene->world = ecs_init_w_args(argc, argv);

	ECS_COMPONENT_DEFINE(scene->world, pdn_component_transform_t);
	ECS_COMPONENT_DEFINE(scene->world, pdn_component_sprite_t);
	ECS_COMPONENT_DEFINE(scene->world, pdn_component_tilemap_t);

	// TODO: Delete tilemap components with dtor.

	return PDN_RESULT_OK;
}

void pdn_scene_delete(struct pdn_scene* scene) {
	ecs_fini(scene->world);
}
