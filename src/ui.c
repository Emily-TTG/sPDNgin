#include <pdn/ui.h>
#include <pdn/detail.h>
#include <pdn/context.h>
#include <pdn/settings.h>
#include <pdn/log.h>

static enum pdn_result pdn_ui_load_style(struct pdn_ui* ui, const char* path) {
	// TODO: Serialize more style elements than global colors.
	ALLEGRO_FILE* file = al_fopen(path, "r");
	if(!file) {
		return PDN_LOG_RESULT_PATH(al_fopen, path, PDN_RESULT_ERROR);
	}

	static struct nk_color color_table[NK_COLOR_COUNT];

	size_t read_count = al_fread(file, color_table, sizeof(color_table));
	if(read_count < NK_COLOR_COUNT) {
		if(al_feof(file)) {
			// TODO: `fclose` sets `errno` on failure.
			if(al_fclose(file)) {
				(void) PDN_LOG_RESULT(al_fclose, PDN_RESULT_ERROR);
			}

			return PDN_RESULT_EOF;
		}

		if(al_ferror(file)) {
			if(al_fclose(file)) {
				(void) PDN_LOG_RESULT(al_fclose, PDN_RESULT_ERROR);
			}

			return PDN_RESULT_ERROR;
		}

		return PDN_RESULT_ERROR;
	}

	nk_style_from_table(ui->context, color_table);

	return PDN_RESULT_OK;
}

enum pdn_result pdn_ui_new(struct pdn_ui* ui, struct pdn_context* context) {
	enum pdn_result result;

	auto settings = context->settings;

	// TODO: Upstream method to create font from existing Nuklear font?
	// TODO: Can this load a font from PhysFS?
	// TODO: Font size/scaling config.
	// TODO: Separate out font loading infrastructure.
	const char* path = "res/font/unitblock/unitblock.ttf";
	ui->font = nk_allegro5_font_create_from_file(path, 18, 0);
	if(!ui->font) {
		return PDN_LOG_RESULT_PATH(al_fopen, path, PDN_RESULT_ERROR);
	}

	ui->context = nk_allegro5_init(
			ui->font, context->display->display,
			settings->width, settings->height);

	result = pdn_ui_load_style(ui, "res/style/game_nk_style.bin");
	if(result) {
		(void) PDN_LOG_RESULT(pdn_ui_load_style, result);
	}

	return PDN_RESULT_OK;
}

void pdn_ui_delete(struct pdn_ui* ui) {
	nk_allegro5_font_del(ui->font);

	nk_allegro5_shutdown();
}
