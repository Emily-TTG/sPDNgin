#include <game/log.h>

#include <stdio.h>

void gm_log_variadic(
		const char* function, const char* file, int line,
		const char* format, va_list list) {

	int written;

	written = printf("%s:%d in %s(): ", file, line, function);
	if(written < 0) {
		perror("printf");
	}

	written = vprintf(format, list);
	if(written < 0) {
		perror("printf");
	}

	if(putchar('\n') == EOF) {
		perror("putchar");
	}
}

void gm_log(
		const char* function, const char* file, int line,
		const char* format, ...) {

	va_list list;
	va_start(list);
	gm_log_variadic(function, file, line, format, list);
	va_end(list);
}

void gm_log_result(
		const char* function, const char* file, int line,
		const char* context, const char* path, enum gm_result result) {

	gm_log(
			function, file, line,
			"%s(%s): %s",
			context, path ? path : "", gm_result_description(result));
}
