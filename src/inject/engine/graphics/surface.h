#pragma once

#include <stdint.h>
#include <ddraw.h>

typedef struct ZGfxSurface {
	uint32_t width;
	uint32_t height;
	uint32_t pitch;
	uint32_t unk0;
	void* surfaceMemory;
	bool isLocked;
	uint32_t unk1;
	uint32_t unk2;
	IDirectDrawSurface3* ddSurface;
} ZGfxSurface;
_STATIC_ASSERT(sizeof(ZGfxSurface) == 0x24);

HRESULT __fastcall ZGfxLockDDSurface(IDirectDrawSurface3* ddSurface, DDSURFACEDESC2* surfaceDesc);
HRESULT __fastcall ZGfxUnlockDDSurface(IDirectDrawSurface3* ddSurface);

HRESULT __fastcall ZGfxLockSurface(ZGfxSurface* surface);
HRESULT __fastcall ZGfxUnlockSurface(ZGfxSurface* surface);

