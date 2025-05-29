#include <game/local/update.h>

#include <game/context.h>
#include <game/detail.h>
#include <game/log.h>

enum gm_result gm_local_render(struct gm_context* context) {
	auto ui = context->ui;

	struct nk_rect area = nk_rect(50, 50, 220, 220);
	if(nk_begin(ui->context, "game", area, NK_WINDOW_BORDER)) {
		nk_layout_row_static(ui->context, 30, 80, 1);
		if(nk_button_label(ui->context, "button")) {
			GM_LOG("Hello, Nuklear!");
		}
	}
	nk_end(ui->context);

	return GM_RESULT_OK;
}
