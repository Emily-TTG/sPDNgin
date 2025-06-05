#include <pdn/result.h>

const char* pdn_result_description(enum pdn_result result) {
	switch(result) {
		default: break;
		case PDN_RESULT_OK: return "no error";
		case PDN_RESULT_ERROR: return "unknown error";
		case PDN_RESULT_EOF: return "end of file";
		case PDN_RESULT_BAD_TYPE: return "bad type";
	}

	return "unknown";
}
