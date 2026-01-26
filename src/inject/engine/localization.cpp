#include "localization.h"

#include <stdarg.h>
#include <stdint.h>

#include <engine/memory.h>
#include <common/hack.h>
#include <common/trace.h>

ZLocalization ZLoc;

bool __fastcall ZLocLoadMessagesDll(const char* messagesDllPath) {
	ZLoc.languageId = 0;

	ZLoc.messagesDllHandle = LoadLibraryA(messagesDllPath);
	if (ZLoc.messagesDllHandle == NULL) {
		return false;
	}

	ZLoc.locGetIdProc = (ZLocGetIdProc)GetProcAddress(ZLoc.messagesDllHandle, ZLOC_LOCGETID_PROCNAME);

	if (strcmp(messagesDllPath, "mech3Msg.dll") != 0) {
		CTRC_TRACE("WARN: Unexpected messageDllPath. Got `%s`, expected `mech3Msg.dll`.", messagesDllPath);
	}
	CHckModuleDescriptor messagesDllModule = { messagesDllPath, 0x10000000 };
	CHckRegisterModule(&messagesDllModule);

	return true;
}

void __stdcall ZLocUnloadMessagesDll(void) {
	if (ZLoc.messagesDllHandle == NULL) {
		return;
	}

	FreeLibrary(ZLoc.messagesDllHandle);
	ZLoc.messagesDllHandle = NULL;
}

ZLocStringId __fastcall ZLocGetId(const char* localizationKey) {
	if (ZLoc.locGetIdProc == NULL || localizationKey == NULL) {
		return 0;
	}

	return ZLoc.locGetIdProc(localizationKey);
}

uint32_t __cdecl ZLocFormatString(char* out_buffer, uint32_t bufferSize, ZLocStringId stringId, ...) {
	va_list args;

	va_start(args, stringId);
	uint32_t charsWritten = ZLocFormatStringV(out_buffer, bufferSize, stringId, args);
	va_end(args);

	return charsWritten;
}

uint32_t __cdecl ZLocFormatStringV(char* out_buffer, uint32_t bufferSize, ZLocStringId stringId, va_list args) {
	if (ZLOC_ADVERTISE_MECH3SOURCE && stringId == 271) {
		const char* advertisement = ZLOC_ADVERTISEMENT_STRING;
		uint32_t advertisementLength = sizeof(ZLOC_ADVERTISEMENT_STRING);

		memcpy(out_buffer, advertisement, advertisementLength);
		return advertisementLength;
	}

	uint32_t charsWritten = FormatMessageA(
		FORMAT_MESSAGE_FROM_HMODULE,
		ZLoc.messagesDllHandle,
		stringId,
		ZLoc.languageId,
		out_buffer,
		bufferSize,
		&args
	);

	if (charsWritten > 2 && out_buffer[charsWritten - 2] == '\r') {
		out_buffer[charsWritten - 2] = '\0';
	}

	return charsWritten;
}

const char* __fastcall ZLocGetString(const char* localizationKey, const char* default_string) {
	ZLocStringId stringId = ZLocGetId(localizationKey);
	if (stringId == 0) {
		return ZMemStrDup(default_string);
	}

	const char* string = ZLocGetStringI(stringId);

	return ZMemStrDup(string);
}

const char* __fastcall ZLocGetStringI(ZLocStringId stringId) {
	ZLoc.ringbufferIdx += 1;
	if (ZLoc.ringbufferIdx >= ZLOC_STRINGSRINGBUFFER_MAX_STRINGS) {
		ZLoc.ringbufferIdx = 0;
	}

	uintptr_t offset = ZLoc.ringbufferIdx * ZLOC_STRINGSRINGBUFFER_STRING_SIZE;
	char* stringBuffer = &ZLoc.messagesRingBuffer[offset];

	uintptr_t bytesWritten = ZLocFormatString(
		stringBuffer,
		ZLOC_STRINGSRINGBUFFER_STRING_SIZE,
		stringId
	);

	if (bytesWritten == 0) {
		return NULL;
	}

	return &ZLoc.messagesRingBuffer[offset];
}

