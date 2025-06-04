#include <game/local/local.h>

#include <game/context.h>
#include <game/detail.h>
#include <game/log.h>
#include <game/components.h>
#include <game/script.h>
#include <game/tilemap.h>

ECS_SYSTEM_DECLARE(gm_local_render_tilemap);
ECS_SYSTEM_DECLARE(gm_local_render_sprite);

static void gm_local_render_tilemap(ecs_iter_t* it) {
	auto transforms = ecs_field(it, gm_component_transform_t, 0);
	auto tilemaps = ecs_field(it, gm_component_tilemap_t, 1);

	for(int i = 0; i < it->count; ++i) {
		auto transform = &transforms[i];
		auto tilemap = &tilemaps[i];

		auto tileset = &tilemap->tilemap.tileset;
		int dimension = tileset->dimension;

		for(int y = 0; y < tilemap->tilemap.height; ++y) {
			for(int x = 0; x < tilemap->tilemap.width; ++x) {
				int global_id =
						tilemap->tilemap.data[y * tilemap->tilemap.width + x];

				bool horizontal = global_id & (1 << 31);
				bool vertical = global_id & (1 << 30);

				int id = (global_id & 0xFFFFFFF) - 1;
				int tile_column = id % tileset->width;
				int tile_row = id / tileset->width;

				float sx = (float) (tile_column * dimension);
				float sy = (float) (tile_row * dimension);
				float sw = (float) dimension;
				float sh = (float) dimension;

				float dx = (float) (x * dimension);
				float dy = (float) (y * dimension);

				int flags = 0;
				if(horizontal) {
					flags |= ALLEGRO_FLIP_HORIZONTAL;
				}
				else if(vertical) {
					flags |= ALLEGRO_FLIP_VERTICAL;
				}

				al_draw_tinted_scaled_rotated_bitmap_region(
						tileset->atlas, sx, sy, sw, sh,
						al_map_rgb(255, 255, 255),
						sw / 2.0f, sh / 2.0f,
						transform->x + dx, transform->y + dy,
						1.0f, 1.0f, 0.0f, flags);
			}
		}
	}
}

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
			world, gm_local_render_tilemap, EcsOnUpdate,
			gm_component_transform_t, gm_component_tilemap_t);

	ECS_SYSTEM_DEFINE(
			world, gm_local_render_sprite, EcsOnUpdate,
			gm_component_transform_t, gm_component_sprite_t);

	ecs_entity_t test = ecs_entity(world, { .name = "test" });
	ecs_set(world, test, gm_component_transform_t, { 50.0f, 50.0f });
	ecs_set(
			world, test, gm_component_sprite_t,
			{ al_load_bitmap("res/img/arse.png") });


	ecs_entity_t test2 = ecs_entity(world, { .name = "test2" });
	ecs_set(world, test2, gm_component_transform_t, { 0.0f, 0.0f });
	struct gm_tilemap tilemap;
	gm_script_file_table(
			"res/map/map.lua", gm_tilemap_script_table_handler, &tilemap);

	ecs_set(world, test2, gm_component_tilemap_t, { tilemap });

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
