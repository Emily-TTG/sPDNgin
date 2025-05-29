#include <game/local/local.h>

#include <game/context.h>
#include <game/detail.h>
#include <game/log.h>

#include <stdio.h>

static ALLEGRO_BITMAP* test_bit;

enum gm_result gm_local_start_render(struct gm_context* context) {
	(void) context;

	test_bit = al_load_bitmap("res/img/arse.png");

	return GM_RESULT_OK;
}

enum gm_result gm_local_tick_render(struct gm_context* context) {
	auto ui = context->ui;

	al_set_target_backbuffer(context->display->display);
	al_clear_to_color(al_map_rgb(0, 0, 0));

	al_draw_bitmap(test_bit, 50.0f, 50.0f, 0);

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
