#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

typedef struct {
	FILE* traceFile;
} ZTrace;
extern ZTrace ZTrc;

void ZTrcInit(void);
void ZTrcDeinit(void);

void ZTrcTrace(const char* format, ...);
void ZTrcFlush(void);

#define ZTRC_TRACE(_format, ...) do { ZTrcTrace("[%s:%d] - " _format "\n", __FUNCTION__, __LINE__, __VA_ARGS__); } while(0)

