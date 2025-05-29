#pragma once

#include <game/result.h>

#include <stdarg.h>

void gm_log_variadic(const char*, const char*, int, const char*, va_list);
void gm_log(const char*, const char*, int, const char*, ...);
#define GM_LOG(format, ...) \
		gm_log(__func__, __FILE__, __LINE__, format __VA_OPT__(,) __VA_ARGS__)

void gm_log_result(const char*, const char*, int, const char*, enum gm_result);
#define GM_LOG_RESULT_LOCATION(function, result) \
		gm_log_result(__func__, __FILE__, __LINE__, #function, result)

#define GM_LOG_RESULT(function, result) \
		(GM_LOG_RESULT_LOCATION(function, result), result)
