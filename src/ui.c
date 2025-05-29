#include <game/ui.h>
#include <game/detail.h>
#include <game/context.h>
#include <game/settings.h>

enum gm_result gm_ui_new(struct gm_ui* ui, struct gm_context* context) {
	auto settings = context->settings;

	// TODO: Upstream method to create font from existing Nuklear font?
	// TODO: Can this load a font from PhysFS?
	// TODO: Font size/scaling config.
	ui->font = nk_allegro5_font_create_from_file(
			"res/font/unitblock/unitblock.ttf", 13, 0);

	ui->context = nk_allegro5_init(
			ui->font, context->display->display,
			settings->width, settings->height);

	return GM_RESULT_OK;
}

void gm_ui_delete(struct gm_ui* ui) {
	nk_allegro5_font_del(ui->font);

	nk_allegro5_shutdown();
}
