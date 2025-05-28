#pragma once

#include <game/result.h>

enum gm_sync_mode {
	GM_SYNC_UNSET, // Allow display backend to decide optimal sync settings.
	GM_SYNC_EXPLICIT, // Explicitly requested framerate.
	GM_SYNC_VSYNC // V-Synced framerate.
};

enum gm_display_mode {
	GM_DISPLAY_UNSET,
	GM_DISPLAY_WINDOWED,
	GM_DISPLAY_WINDOWED_FULLSCREEN,
	GM_DISPLAY_FULLSCREEN
};

// TODO: Graphics API controls.
struct gm_settings {
	enum gm_sync_mode sync_mode;
	unsigned framerate;

	enum gm_display_mode display_mode;
	// TODO: How should these be respected in windowed fullscreen mode?
	unsigned width, height;
};

enum gm_result gm_settings_load(struct gm_settings*, int, char**);
