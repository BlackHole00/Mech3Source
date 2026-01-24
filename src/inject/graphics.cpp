#include "graphics.h"

#include <ddraw.h>
#include <dinput.h>

typedef HRESULT (WINAPI *DirectDrawCreateProc)(GUID FAR *lpGUID, LPDIRECTDRAW FAR *lplpDD, IUnknown FAR *pUnkOuter);
typedef HRESULT (WINAPI *DirectInputCreateAProc)(HINSTANCE hInstance, DWORD dwVersion, LPDIRECTINPUTA* ppDi, LPUNKNOWN punkOther);

const GUID DECLSPEC_SELECTANY IID_IDirectDrawSurface3	= { 0xDA044E00, 0x69B2, 0x11D0, { 0xA1, 0xD5, 0x00, 0xAA, 0x00, 0xB8, 0xDF, 0xBB } };
const GUID DECLSPEC_SELECTANY IID_IDirectDrawSurface4	= { 0x0B2B8630, 0xAD35, 0x11D0, { 0x8E, 0xA6, 0x00, 0x60, 0x97, 0x97, 0xEA, 0x5B } };
const GUID DECLSPEC_SELECTANY IID_IDirectDraw2		= { 0xB3A6F3E0, 0x2B43, 0x11CF, { 0xA2, 0xDE, 0x00, 0xAA, 0x00, 0xB9, 0x33, 0x56 } };

void ZGfxCheckCapabilities9x(ZGfxCapabilityLevel* capabilityLevel, ZGfxInitializationMode* initializationMode) {
	*initializationMode = ZGFX_INIT_WITH_DDRAW;

	HMODULE directDrawHandle = LoadLibraryA("DDRAW.DLL");
	if (directDrawHandle == NULL) {
		*capabilityLevel = ZGFX_CAPABILITY_NONE;
		*initializationMode = ZGFX_INIT_IMPOSSIBLE;
		return;
	}

	DirectDrawCreateProc directDrawCreate = (DirectDrawCreateProc)GetProcAddress(directDrawHandle, "DirectDrawCreate");
	if (directDrawCreate == NULL) {
		*capabilityLevel = ZGFX_CAPABILITY_NONE;
		*initializationMode = ZGFX_INIT_IMPOSSIBLE;
		goto cleanup1;
	}

	IDirectDraw* directDraw;
	if (directDrawCreate(0, &directDraw, NULL) != S_OK) {
		*capabilityLevel = ZGFX_CAPABILITY_NONE;
		*initializationMode = ZGFX_INIT_IMPOSSIBLE;
		goto cleanup1;
	}

	IDirectDraw2* directDraw2;
	if (directDraw->QueryInterface(IID_IDirectDraw2, (void**)&directDraw2) != S_OK) {
		goto cleanup2;
	}
	directDraw2->Release();


	*capabilityLevel = ZGFX_HAS_DDRAW;

	HMODULE directInputHandle = LoadLibraryA("DINPUT.DLL");
	if (directInputHandle == NULL) {
		goto cleanup2;
	}

	DirectInputCreateAProc directInputCreateA = (DirectInputCreateAProc)GetProcAddress(directInputHandle, "DirectInputCreateA");
	if (directInputCreateA == NULL) {
		goto cleanup3;
	}

	*capabilityLevel = ZGFX_HAS_DINPUT;

	
	if (directDraw->SetCooperativeLevel(NULL, DDSCL_NORMAL) != S_OK) {
		*capabilityLevel = ZGFX_CAPABILITY_NONE;
		*initializationMode = ZGFX_INIT_IMPOSSIBLE;
		goto cleanup3;
	}

	IDirectDrawSurface* surface;
	DDSURFACEDESC surfaceDesc = {};
	surfaceDesc.dwSize = sizeof(DDSURFACEDESC);
	surfaceDesc.dwFlags = DDSD_CAPS;
	surfaceDesc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
	if (directDraw->CreateSurface(&surfaceDesc, &surface, NULL) != S_OK) {
		*capabilityLevel = ZGFX_CAPABILITY_NONE;
		*initializationMode = ZGFX_INIT_IMPOSSIBLE;
		goto cleanup3;
	}


	IDirectDrawSurface3* surface3;
	if (surface->QueryInterface(IID_IDirectDrawSurface3, (void**)&surface3) != S_OK) {
		goto cleanup4;
	}

	*capabilityLevel = ZGFX_HAS_IDIRECTDRAWSURFACE3;


	IDirectDrawSurface4* surface4;
	if (surface->QueryInterface(IID_IDirectDrawSurface4, (void**)&surface4) != S_OK) {
		goto cleanup4;
	}

	*capabilityLevel = ZGFX_HAS_IDIRECTDRAWSURFACE4;


cleanup4:
	surface->Release();
cleanup3:
	FreeLibrary(directInputHandle);
cleanup2:
	directDraw->Release();
cleanup1:
	FreeLibrary(directDrawHandle);
}

void ZGfxCheckCapabilitiesNT(uint32_t ntVersion, ZGfxCapabilityLevel* capabilityLevel, ZGfxInitializationMode* initializationMode) {
	if (ntVersion < 4) {
		*capabilityLevel = ZGFX_CAPABILITY_NONE;
		*initializationMode = ZGFX_INIT_IMPOSSIBLE;
		return;
	}

	if (ntVersion == 4) {
		HMODULE directInputHandle = LoadLibraryA("DINPUT.DLL");
		if (directInputHandle == NULL) {
			*capabilityLevel = ZGFX_CAPABILITY_NONE;
			*initializationMode = ZGFX_INIT_IMPOSSIBLE;
			return;
		}

		DirectDrawCreateProc directDrawCreate = (DirectDrawCreateProc)GetProcAddress(directInputHandle, "DirectDrawCreate");
		FreeLibrary(directInputHandle);

		if (directDrawCreate == NULL) {
			*capabilityLevel = ZGFX_CAPABILITY_NONE;
			*initializationMode = ZGFX_INIT_IMPOSSIBLE;
			return;
		}

		*initializationMode = ZGFX_INIT_WITH_DINPUT;
		*capabilityLevel = ZGFX_HAS_DINPUT;
		return;
	}

	ZGfxCheckCapabilities9x(capabilityLevel, initializationMode);
}

void __fastcall ZGfxCheckCapabilities(ZGfxCapabilityLevel* capabilityLevel, ZGfxInitializationMode* initializationMode) {
	if (ZGFX_DISABLE_PESKY_CAPABILITY_CHECK) {
		*capabilityLevel = ZGFX_FULL_CAPABILITY;
		*initializationMode = ZGFX_INIT_WITH_DDRAW;
		return;
	}

	OSVERSIONINFOA versionInfo;
	versionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);

	if (!GetVersionEx(&versionInfo)) {
		*capabilityLevel = ZGFX_CAPABILITY_NONE;
		*initializationMode = ZGFX_INIT_IMPOSSIBLE;
		return;
	}

	DirectDrawCreateProc directDrawCreate;
	if (versionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT) {
		ZGfxCheckCapabilitiesNT(versionInfo.dwMajorVersion, capabilityLevel, initializationMode);
	} else {
		ZGfxCheckCapabilities9x(capabilityLevel, initializationMode);
	}
}

