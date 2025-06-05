#pragma once

enum pdn_result {
	PDN_RESULT_OK,
	PDN_RESULT_ERROR,
	PDN_RESULT_EOF,
	PDN_RESULT_BAD_TYPE
};

const char* pdn_result_description(enum pdn_result);
