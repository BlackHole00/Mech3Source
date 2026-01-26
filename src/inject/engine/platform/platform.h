#pragma once

#include <stdint.h>
#include <Windows.h>

// typedef class ZPltWinApp : public CWinApp {
// } ZPltWinApp;

typedef struct ZPlatform {
	LARGE_INTEGER startTime;
} ZPlatform;
extern ZPlatform ZPlt;

void ZPltInit(void);

uint32_t ZPltGetTickCount(void);

bool ZPltSetupCurrentWorkingDirectory(void);
