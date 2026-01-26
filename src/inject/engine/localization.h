#pragma once

#include <corecrt.h>
#include <stdbool.h>
#include <stdint.h>
#include <Windows.h>

#define ZLOC_ADVERTISE_MECH3SOURCE true
#define ZLOC_ADVERTISEMENT_STRING "M E C H W A R R I O R 3\n    S O U R C E"

#define ZLOC_VA 0x0072c6700
#define ZLOC_LOCGETID_PROCNAME "ZLocGetID"
#define ZLOC_STRINGSRINGBUFFER_STRING_SIZE 0x200
#define ZLOC_STRINGSRINGBUFFER_MAX_STRINGS 16
#define ZLOC_STRINGSRINGBUFFER_SIZE ZLOC_STRINGSRINGBUFFER_MAX_STRINGS * ZLOC_STRINGSRINGBUFFER_STRING_SIZE

_STATIC_ASSERT(ZLOC_STRINGSRINGBUFFER_SIZE == 8192);

typedef DWORD ZLocStringId;
typedef DWORD ZLocLanguageId;

typedef ZLocStringId (__cdecl *ZLocGetIdProc)(const char*);

typedef struct {
	ZLocGetIdProc locGetIdProc;

	char messagesRingBuffer[ZLOC_STRINGSRINGBUFFER_SIZE];

	HMODULE messagesDllHandle;

	ZLocLanguageId languageId;

	uint32_t ringbufferIdx;
} ZLocalization;
extern ZLocalization ZLoc;

bool __fastcall ZLocLoadMessagesDll(const char* messagesDllPath);
void __stdcall ZLocUnloadMessagesDll(void);

ZLocStringId __fastcall ZLocGetId(const char* localizationKey);

// From mech3Msg.dll
ZLocStringId __cdecl ZLocGetIdImpl(const char* localizationKey);
ZLocStringId __cdecl ZLocGetLocalizationEntry(const char* localizationKey);

uint32_t __cdecl ZLocFormatString(char* out_buffer, uint32_t bufferSize, ZLocStringId stringId, ...);
uint32_t __cdecl ZLocFormatStringV(char* out_buffer, uint32_t bufferSize, ZLocStringId stringId, va_list args);

const char* __fastcall ZLocGetString(const char* localizationKey, const char* default_string);
const char* __fastcall ZLocGetStringI(ZLocStringId stringId);

