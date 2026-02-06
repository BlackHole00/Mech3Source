#pragma once

#include <sal.h>
#include <stdio.h>

typedef struct {
	FILE* traceFile;
} CTrace;
extern CTrace CTrc;

void CTrcInit(void);
void CTrcDeinit(void);

void CTrcTrace(_Printf_format_string_ const char* format, ...);
void CTrcFlush(void);

#define CTRC_TRACE(_format, ...) do { CTrcTrace("[%s (%s:%d)] " _format "\n", __FUNCTION__, __FILE__, __LINE__, __VA_ARGS__); } while(0)

