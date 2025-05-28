#pragma once

enum gm_result {
	GM_RESULT_OK,
	GM_RESULT_ERROR
};

const char* gm_result_description(enum gm_result);
