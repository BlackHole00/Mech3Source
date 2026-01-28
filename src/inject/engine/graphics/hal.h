#pragma once

#include <Windows.h>

typedef bool (__stdcall *ZGfxInitHalProc)(void);
typedef bool (__stdcall *ZGfxDeinitHalProc)(void);
typedef HRESULT (__fastcall *ZGfxClearSurfaceZBufferProc)(RECT* rect);

// Note: This could very well be a "ZGfxHudHal", since manipulating its functions only has effect to the hud elements
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
	void* proc4;
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

