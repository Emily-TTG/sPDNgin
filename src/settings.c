#include <pdn/settings.h>

enum pdn_result pdn_settings_load(
		struct pdn_settings* settings, int argc, char** argv) {

	(void) argc;
	(void) argv;

	// Sensible defaults.
	*settings = (struct pdn_settings) {
			.sync_mode = PDN_SYNC_UNSET,
			.framerate = 60,

#ifdef NDEBUG
			.display_mode = PDN_DISPLAY_WINDOWED_FULLSCREEN,
#else
			.display_mode = PDN_DISPLAY_UNSET,
#endif
			.width = 1080,
			.height = 720
	};

	// TODO: Graphical display options popup.
	// TODO: Enumerate top-level display modes.
	// TODO: Settings save/load.

	return PDN_RESULT_OK;
}
