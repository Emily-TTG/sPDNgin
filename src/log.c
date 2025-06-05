#include <pdn/log.h>

#include <stdio.h>

#ifdef _WIN32
# include <Windows.h>
#endif

void pdn_log_variadic(
		const char* function, const char* file, int line,
		const char* format, va_list list) {

#ifdef _WIN32
	static thread_local char buffer[BUFSIZ];

	int written;

	written = snprintf(
			buffer, sizeof(buffer), "%s:%d in %s(): ", file, line, function);

	if(written < 0) {
		perror("snprintf");
	}

	written = vsnprintf(
			buffer + written, sizeof(buffer) - written, format, list);

	if(written < 0) {
		perror("vsnprintf");
	}

	OutputDebugStringA(buffer);
#else
	int written;

	written = printf("%s:%d in %s(): ", file, line, function);
	if(written < 0) {
		perror("printf");
	}

	written = vprintf(format, list);
	if(written < 0) {
		perror("vprintf");
	}

	if(putchar('\n') == EOF) {
		perror("putchar");
	}
#endif
}

void pdn_log(
		const char* function, const char* file, int line,
		const char* format, ...) {

	va_list list;
	va_start(list, format);
	pdn_log_variadic(function, file, line, format, list);
	va_end(list);
}

void pdn_log_result(
		const char* function, const char* file, int line,
		const char* context, const char* path, enum pdn_result result) {

	pdn_log(
			function, file, line,
			"%s(%s): %s",
			context, path ? path : "", pdn_result_description(result));
}
