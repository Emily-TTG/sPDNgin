#pragma once

#include <pdn/result.h>

enum pdn_sync_mode {
	PDN_SYNC_UNSET, // Allow display backend to decide optimal sync settings.
	PDN_SYNC_EXPLICIT, // Explicitly requested framerate.
	PDN_SYNC_VSYNC // V-Synced framerate.
};

enum pdn_display_mode {
	PDN_DISPLAY_UNSET,
	PDN_DISPLAY_WINDOWED,
	PDN_DISPLAY_WINDOWED_FULLSCREEN,
	PDN_DISPLAY_FULLSCREEN
};

// TODO: Graphics API controls.
struct pdn_settings {
	enum pdn_sync_mode sync_mode;
	unsigned framerate;

	enum pdn_display_mode display_mode;
	// TODO: How should these be respected in windowed fullscreen mode?
	unsigned width, height;
};

enum pdn_result pdn_settings_load(struct pdn_settings*, int, char**);
