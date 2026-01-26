#pragma once

#include <stdint.h>
#include <ddraw.h>
#include <d3d.h>

#define ZGFX_DISABLE_PESKY_CAPABILITY_CHECK false

typedef enum ZGfxInitializationMode {
	ZGFX_INIT_IMPOSSIBLE = 0,
	ZGFX_INIT_WITH_DDRAW,
	// Refers the the initialization in the old NT 4.x style, where CreateDirectDraw is inside DINPUT.DLL
	ZGFX_INIT_WITH_DINPUT,
} ZGfxInitializationMode;

// NOTE: This enum is created considering only the Windows 9x code path of ZGfxCheckCapabilities. The values do make
//	sense also for the windows NT path, but the returned values of the original decompilation do not match exactly
//	(e.g. ZGFX_HAS_DINPUT gets returned even if the program couldn't load the DINPUT.DLL).
//	Also note that the NT codepath gets executed only for NT 4.x (which is not really supported by the zipper
//	engine). For modern OS versions, the Windows 9x codepath gets executed instead.
typedef enum ZGfxCapabilityLevel {
	ZGFX_CAPABILITY_NONE = 0,
	ZGFX_HAS_DDRAW = 0x200,
	ZGFX_HAS_DINPUT = 0x300,
	ZGFX_HAS_IDIRECTDRAWSURFACE3 = 0x500,
	ZGFX_HAS_IDIRECTDRAWSURFACE4 = 0x600,
	ZGFX_FULL_CAPABILITY = ZGFX_HAS_IDIRECTDRAWSURFACE4,
} ZGfxCapabilityLevel;

void __fastcall ZGfxCheckCapabilities(ZGfxCapabilityLevel* capabilityLevel, ZGfxInitializationMode* initializationMode);

HRESULT __stdcall ZGfxCheckDeviceSuitability(GUID* lpGUID, LPSTR lpDeviceDescription, LPSTR lpDeviceName, D3DDEVICEDESC* lpDeviceDesc, D3DDEVICEDESC* lpHelpDeviceDesc, void* userdata);
