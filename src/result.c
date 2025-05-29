#include <game/result.h>

const char* gm_result_description(enum gm_result result) {
	switch(result) {
		default: break;
		case GM_RESULT_OK: return "no error";
		case GM_RESULT_ERROR: return "unknown error";
		case GM_RESULT_EOF: return "end of file";
	}

	return "unknown";
}
