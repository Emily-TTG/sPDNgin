#pragma once

#include <pdn/result.h>

#include <stdarg.h>

void pdn_log_variadic(const char*, const char*, int, const char*, va_list);
void pdn_log(const char*, const char*, int, const char*, ...);
#define PDN_LOG(format, ...) \
		pdn_log(__func__, __FILE__, __LINE__, format __VA_OPT__(,) __VA_ARGS__)

void pdn_log_result(
		const char*, const char*, int,
		const char*, const char*, enum pdn_result);

#define PDN_LOG_RESULT_LOCATION(function, path, result) \
		pdn_log_result(__func__, __FILE__, __LINE__, #function, path, result)

#define PDN_LOG_RESULT(function, result) \
		(PDN_LOG_RESULT_LOCATION(function, nullptr, result), result)

#define PDN_LOG_RESULT_PATH(function, path, result) \
		(PDN_LOG_RESULT_LOCATION(function, path, result), result)
