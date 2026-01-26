#include "graphics.h"

#include <corecrt.h>
#include <ddraw.h>
#include <d3d.h>
#include <dinput.h>
#include <winnt.h>

#include <common/utils.h>
#include <common/trace.h>
#include <common/hack.h>

typedef HRESULT (WINAPI *DirectDrawCreateProc)(GUID FAR *lpGUID, LPDIRECTDRAW FAR *lplpDD, IUnknown FAR *pUnkOuter);
typedef HRESULT (WINAPI *DirectInputCreateAProc)(HINSTANCE hInstance, DWORD dwVersion, LPDIRECTINPUTA* ppDi, LPUNKNOWN punkOther);

const GUID DECLSPEC_SELECTANY IID_IDirectDrawSurface3	= { 0xDA044E00, 0x69B2, 0x11D0, { 0xA1, 0xD5, 0x00, 0xAA, 0x00, 0xB8, 0xDF, 0xBB } };
const GUID DECLSPEC_SELECTANY IID_IDirectDrawSurface4	= { 0x0B2B8630, 0xAD35, 0x11D0, { 0x8E, 0xA6, 0x00, 0x60, 0x97, 0x97, 0xEA, 0x5B } };
const GUID DECLSPEC_SELECTANY IID_IDirectDraw2		= { 0xB3A6F3E0, 0x2B43, 0x11CF, { 0xA2, 0xDE, 0x00, 0xAA, 0x00, 0xB9, 0x33, 0x56 } };
const GUID DECLSPEC_SELECTANY IID_IDirect3D3		= { 0xBB223240, 0xE72B, 0x11D0, { 0xA9, 0xB4, 0x00, 0xAA, 0x00, 0xC0, 0x99, 0x3E } };

ZGraphics ZGfx;
ZGraphicsExtra ZGfxEx;

void ZGfxInit(void) {
	ZGfxEx.directDraw = (IDirectDraw4**)CHckVirtualAddressToActual(CHCK_DEFAULT_MODULE, 0x00809358);
	ZGfxEx.direct3D = (IDirect3D3**)CHckVirtualAddressToActual(CHCK_DEFAULT_MODULE, 0x0080936c);
}

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

HRESULT __stdcall ZGfxCheckDeviceSuitability(GUID* lpGUID, LPSTR lpDeviceDescription, LPSTR lpDeviceName, D3DDEVICEDESC* lpDeviceDesc, D3DDEVICEDESC* lpHelpDeviceDesc, void* userdata) {
	ZGraphics* gfx = (ZGraphics*)userdata;

	ZGfxD3DDEVICEDESCStorage* currentDescriptor = &gfx->descriptors[gfx->suitableDevicesCount];
	CTRC_TRACE("Found driver: `%s` (%s):", lpDeviceName, lpDeviceDesc);

	if (lpDeviceDesc->dwFlags == 0) {
		CTRC_TRACE("\tSKIPPED - Does not support interface with hardware.");
		return D3DENUMRET_OK;
	}

	if (!(lpDeviceDesc->dwFlags & D3DDD_COLORMODEL) && lpDeviceDesc->dcmColorModel != D3DCOLOR_RGB) {
		CTRC_TRACE("\tSKIPPED - Does not support RGB color.");
		return D3DENUMRET_OK;
	}

	if (!(lpDeviceDesc->dwDeviceZBufferBitDepth & DDBD_16)) {
		CTRC_TRACE("\tSKIPPED - Does not support 16 bit ZBuffer.");
		return D3DENUMRET_OK;
	}

	if (gfx->suitableDevicesCount >= 3) {
		CTRC_TRACE("\tSKIPPED - Already at maximum device count. Stopping enumeration.");
		// TODO: Implements the functions at 0x00578db3
		return D3DENUMRET_CANCEL;
	}

	CTRC_TRACE("\tACCEPTED.");

	if (lpGUID == NULL) {
		currentDescriptor->deviceGuidPtr = NULL;
	} else {
		currentDescriptor->deviceGuidPtr = &currentDescriptor->deviceGuid;
		currentDescriptor->deviceGuid = *lpGUID;
	}

	size_t descriptorSize = CUTL_MIN(sizeof(D3DDEVICEDESC), 252);
	memcpy(&currentDescriptor->descriptor, lpDeviceDesc, descriptorSize);

	if (currentDescriptor->descriptor.dwMaxTextureWidth == 0) {
		currentDescriptor->descriptor.dwMaxTextureWidth = 0x00010000;
	}
	if (currentDescriptor->descriptor.dwMaxTextureHeight == 0) {
		currentDescriptor->descriptor.dwMaxTextureHeight = 0x00010000;
	}

	strncpy(currentDescriptor->deviceName, lpDeviceName, CUTL_COUNTOF_FIELD(ZGfxD3DDEVICEDESCStorage, deviceName));
	strncpy(currentDescriptor->deviceDescriptor, lpDeviceDescription, CUTL_COUNTOF_FIELD(ZGfxD3DDEVICEDESCStorage, deviceDescriptor));
	
	return D3DENUMRET_OK;
}

bool __fastcall ZGfxLoadD3D(ZGraphics* gfx) {
	CTRC_TRACE("Enumerating Devices (%s)...", &gfx->field_0x14);

	if ((*ZGfxEx.directDraw)->QueryInterface(IID_IDirect3D3, (void**)ZGfxEx.direct3D) != S_OK) {
		// TODO: Hook ZGfxHandleError
		return false;
	}

	gfx->suitableDevicesCount = 0;
	(*ZGfxEx.direct3D)->EnumDevices(ZGfxCheckDeviceSuitability, gfx);

	if (*ZGfxEx.direct3D != NULL) {
		(*ZGfxEx.direct3D)->Release();
		*ZGfxEx.direct3D = NULL;
	}

	if (gfx->suitableDevicesCount == 0) {
		CTRC_TRACE("No suitable devices.");
		return false;
	}

	return true;
}

