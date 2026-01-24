#include "trace.h"

#include <stdarg.h>

ZTrace ZTrc;

void ZTrcInit(void) {
	ZTrc.traceFile = fopen("trace.log", "w");
}

void ZTrcDeinit(void) {
	fflush(ZTrc.traceFile);
	fclose(ZTrc.traceFile);
}

void ZTrcTrace(const char* format, ...) {
	va_list args;
	va_start(args, format);

	vfprintf(ZTrc.traceFile, format, args);

	va_end(args);

	fflush(ZTrc.traceFile);
}

void ZTrcFlush(void) {
	fflush(ZTrc.traceFile);
}


