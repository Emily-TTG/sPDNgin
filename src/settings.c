#include <game/settings.h>

enum gm_result gm_settings_load(
		struct gm_settings* settings, int argc, char** argv) {

	(void) argc;
	(void) argv;

	// Sensible defaults.
	*settings = (struct gm_settings) {
			.sync_mode = GM_SYNC_UNSET,
#ifdef NDEBUG
			.display_mode = GM_DISPLAY_WINDOWED_FULLSCREEN,
#else
			.display_mode = GM_DISPLAY_UNSET,
#endif
			.width = 1080,
			.height = 720
	};

	// TODO: Graphical display options popup.
	// TODO: Enumerate top-level display modes.
	// TODO: Settings save/load.

	return GM_RESULT_OK;
}
