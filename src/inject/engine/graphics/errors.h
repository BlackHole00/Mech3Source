#pragma once

#include <stdint.h>
#include <Windows.h>

#define ZGFX_HANDLE_ERROR(_result) do { ZGfxHandleError(_result, __FILE__, __LINE__); } while (0);

uint32_t __fastcall ZGfxHandleError(HRESULT result, const char* sourceCodePath, uint32_t lineNumber);
