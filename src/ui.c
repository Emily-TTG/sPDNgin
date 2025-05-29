#include <game/ui.h>
#include <game/detail.h>
#include <game/context.h>
#include <game/settings.h>
#include <game/log.h>

static enum gm_result gm_ui_load_style(struct gm_ui* ui, const char* path) {
	// TODO: Serialize more style elements than global colors.
	ALLEGRO_FILE* file = al_fopen(path, "r");
	if(!file) {
		return GM_LOG_RESULT_PATH(al_fopen, path, GM_RESULT_ERROR);
	}

	static struct nk_color color_table[NK_COLOR_COUNT];

	size_t read_count = al_fread(file, color_table, sizeof(color_table));
	if(read_count < NK_COLOR_COUNT) {
		if(al_feof(file)) {
			// TODO: `fclose` sets `errno` on failure.
			if(al_fclose(file)) {
				(void) GM_LOG_RESULT(al_fclose, GM_RESULT_ERROR);
			}

			return GM_RESULT_EOF;
		}

		if(al_ferror(file)) {
			if(al_fclose(file)) {
				(void) GM_LOG_RESULT(al_fclose, GM_RESULT_ERROR);
			}

			return GM_RESULT_ERROR;
		}

		return GM_RESULT_ERROR;
	}

	nk_style_from_table(ui->context, color_table);

	return GM_RESULT_OK;
}

enum gm_result gm_ui_new(struct gm_ui* ui, struct gm_context* context) {
	enum gm_result result;

	auto settings = context->settings;

	// TODO: Upstream method to create font from existing Nuklear font?
	// TODO: Can this load a font from PhysFS?
	// TODO: Font size/scaling config.
	// TODO: Separate out font loading infrastructure.
	const char* path = "res/font/unitblock/unitblock.ttf";
	ui->font = nk_allegro5_font_create_from_file(path, 13, 0);
	if(!ui->font) {
		return GM_LOG_RESULT_PATH(al_fopen, path, GM_RESULT_ERROR);
	}

	ui->context = nk_allegro5_init(
			ui->font, context->display->display,
			settings->width, settings->height);

	result = gm_ui_load_style(ui, "res/style/game_nk_style.bin");
	(void) GM_LOG_RESULT(gm_ui_load_style, result);

	return GM_RESULT_OK;
}

void gm_ui_delete(struct gm_ui* ui) {
	nk_allegro5_font_del(ui->font);

	nk_allegro5_shutdown();
}
