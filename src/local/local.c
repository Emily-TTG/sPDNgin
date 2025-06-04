#include <game/local/local.h>

#include <game/context.h>
#include <game/detail.h>
#include <game/log.h>
#include <game/components.h>
#include <game/script.h>
#include <game/tilemap.h>

ECS_SYSTEM_DECLARE(gm_local_render_sprite);
static void gm_local_render_sprite(ecs_iter_t* it) {
	auto transforms = ecs_field(it, gm_component_transform_t, 0);
	auto sprites = ecs_field(it, gm_component_sprite_t, 1);

	for(int i = 0; i < it->count; ++i) {
		auto transform = &transforms[i];
		auto sprite = &sprites[i];

		al_draw_bitmap(sprite->bitmap, transform->x, transform->y, 0);
	}
}

enum gm_result gm_local_start(struct gm_context* context) {
	auto world = context->scene->world;

	ECS_SYSTEM_DEFINE(
			world, gm_local_render_sprite, EcsOnUpdate,
			gm_component_transform_t, gm_component_sprite_t);

	ecs_entity_t test = ecs_entity(world, { .name = "test" });
	ecs_set(world, test, gm_component_transform_t, { 50.0f, 50.0f });
	const char* path = "res/img/arse.png";
	ecs_set(world, test, gm_component_sprite_t, { al_load_bitmap(path) });

	struct gm_tileset tileset;
	gm_script_file_table(
			"res/tile/goop.lua", gm_tileset_script_table_handler, &tileset);

	struct gm_tilemap tilemap;
	gm_script_file_table(
			"res/map/test.lua", gm_tilemap_script_table_handler, &tilemap);

	return GM_RESULT_OK;
}

enum gm_result gm_local_update(struct gm_context* context, double delta) {
	auto ui = context->ui;

	al_set_target_backbuffer(context->display->display);
	al_clear_to_color(al_map_rgb(0, 0, 0));

	ecs_progress(context->scene->world, (float) delta);

	struct nk_rect area = nk_rect(50, 50, 220, 220);
	enum nk_panel_flags flags =
			NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_TITLE;

	if(nk_begin(ui->context, "game", area, flags)) {
		nk_layout_row_static(ui->context, 30, 80, 1);
		if(nk_button_label(ui->context, "button")) {
			GM_LOG("Hello, Nuklear!");
		}
	}
	nk_end(ui->context);

	return GM_RESULT_OK;
}
