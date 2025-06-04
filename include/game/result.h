#pragma once

enum gm_result {
	GM_RESULT_OK,
	GM_RESULT_ERROR,
	GM_RESULT_EOF,
	GM_RESULT_BAD_TYPE
};

const char* gm_result_description(enum gm_result);
