#pragma once

#include <stdint.h>
#include <Windows.h>

#define ZGFX_STRICT_ERRORS false

#define ZGFX_HANDLE_ERROR(_result) ZGfxHandleError(_result, __FILE__, __LINE__)

uint32_t __fastcall ZGfxHandleError(HRESULT result, const char* sourceCodePath, uint32_t lineNumber);
