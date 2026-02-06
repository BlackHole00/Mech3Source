#pragma once

#include <Windows.h>

#include <common/types.h>
#include <engine/graphics/errors.h>

typedef ZGfxError (__stdcall *ZGfxInitHalProc)(void);
typedef ZGfxError (__stdcall *ZGfxDeinitHalProc)(void);
typedef HRESULT (__fastcall *ZGfxClearSurfaceZBufferProc)(RECT* rect);
typedef ZGfxError (__stdcall *ZGfxApplyResolutionProc)(void);

typedef struct ZGfxHal {
	// 0x00809324
	ZGfxInitHalProc initHal;
	// 0x00809328
	ZGfxDeinitHalProc deinitHal;
	// 0x0080932c
	void* proc1;
	// 0x00809330
	void* proc2;
	// 0x00809334
	void* proc3;
	// 0x00809338
	ZGfxApplyResolutionProc applyResolution;
	// 0x0080933c
	void* proc5;
	// 0x0080933c
	void* proc6;
	// 0x00809340
	void* proc7;
	// 0x00809344
	void* proc8;
	// 0x00809348
	ZGfxClearSurfaceZBufferProc clearSurfaceZBuffer;
} ZGfxHal;

HRESULT __fastcall ZGfxClearSurfaceZBuffer(RECT* rect);

void __stdcall ZGfxEvictTextures(void);
bool __stdcall ZGfxEnterFullscreen(void);

HRESULT __stdcall ZGfxBeginScene(void);
void __fastcall ZGfxSetFog(bool32_t enabled);
void __stdcall ZGfxSetFogColor(void);

typedef enum ZGfxResolution {
	ZGFX_UNKNOWN_RESOLUTION_0	= 0,
	ZGFX_UNKNOWN_RESOLUTION_1	= 1,
	ZGFX_640X400_HALFDEPTH		= 2,
	ZGFX_640X480_HALFDEPTH		= 3,
	ZGFX_640X400			= 4,
	ZGFX_640X480			= 5,
	ZGFX_800X600			= 6,
	ZGFX_1024X768			= 7,
	ZGFX_1152X864			= 8,
	ZGFX_1280X1024			= 9,
	ZGFX_1600X1200			= 10
} ZGfxResolution;

void __fastcall ZGfxChangeResolution(ZGfxResolution resolution);
ZGfxError __fastcall ZGfxChangeResolutionS(ZGfxResolution resolution); // safe

// Applies the desired resolution width and height to the surfaces, but does not recreate the surfaces
void __fastcall ZGfxSetupResolution(ZGfxResolution resolution);
