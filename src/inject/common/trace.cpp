#include "trace.h"

#include <stdarg.h>

CTrace CTrc;

void CTrcInit(void) {
	CTrc.traceFile = fopen("trace.log", "w");
}

void CTrcDeinit(void) {
	fflush(CTrc.traceFile);
	fclose(CTrc.traceFile);
}

void CTrcTrace(const char* format, ...) {
	va_list args;
	va_start(args, format);

	vfprintf(CTrc.traceFile, format, args);

	va_end(args);

	fflush(CTrc.traceFile);
}

void CTrcFlush(void) {
	fflush(CTrc.traceFile);
}


