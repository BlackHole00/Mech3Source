#include "platform.h"

#include <stdio.h>
#include <Windows.h>

#include <game/game.h>
#include <common/utils.h>

ZPlatform ZPlt;

void ZPltInit(void) {
	QueryPerformanceCounter(&ZPlt.startTime);
}

bool ZPltSetupCurrentWorkingDirectory(void) {
	char buffer[64];

	const char* version = GGamGetVersionString();
	snprintf(buffer, CUTL_COUNTOF(buffer), "Software\\Zipper\\MechWarrior3\\%s", version);

	HKEY key;
	if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, buffer, 0, KEY_EXECUTE, &key) != ERROR_SUCCESS) {
		return false;
	}

	BYTE keyBuffer[64];
	DWORD keyBufferLength = CUTL_COUNTOF(keyBuffer);
	DWORD type = REG_SZ;
	if (RegQueryValueExA(key, "RunDir", 0, &type, keyBuffer, &keyBufferLength) != ERROR_SUCCESS) {
		return false;
	}

	SetCurrentDirectoryA((LPCSTR)keyBuffer);

	RegCloseKey(key);

	return true;
}

// Courtesy of ZipperFixup
uint32_t ZPltGetTickCount(void) {
	LARGE_INTEGER performanceCounter;
	LARGE_INTEGER performanceFrequency;

	QueryPerformanceFrequency(&performanceFrequency);
	QueryPerformanceCounter(&performanceCounter);

	int64_t countsDiff = performanceCounter.QuadPart - ZPlt.startTime.QuadPart;
	int64_t currentMilliseconds = countsDiff * 1000 / performanceFrequency.QuadPart;

	return (uint32_t)currentMilliseconds;
}

