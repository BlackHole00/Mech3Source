#pragma once

#include <stdio.h>

typedef struct {
	FILE* traceFile;
} CTrace;
extern CTrace CTrc;

void CTrcInit(void);
void CTrcDeinit(void);

void CTrcTrace(const char* format, ...);
void CTrcFlush(void);

#define CTRC_TRACE(_format, ...) do { CTrcTrace("[%s:%d] - " _format "\n", __FUNCTION__, __LINE__, __VA_ARGS__); } while(0)

