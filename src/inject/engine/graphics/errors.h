#pragma once

#include <stdint.h>
#include <Windows.h>

#define ZGFX_STRICT_ERRORS false

typedef enum ZGfxError {
	ZGFX_GENERIC_FAILURE	= 0x00000000,
	ZGFX_SUCCESS		= 0x00000001,
	ZGFX_NOT_INITIALIZED	= 0x5a560000,
} ZGfxError;

#define ZGFX_HANDLE_ERROR(_result) ZGfxHandleError(_result, __FILE__, __LINE__)
uint32_t __fastcall ZGfxHandleError(HRESULT result, const char* sourceCodePath, uint32_t lineNumber);
