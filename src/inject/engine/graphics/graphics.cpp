#include "graphics.h"

#include <corecrt.h>
#include <ddraw.h>
#include <winnt.h>

#include <common/hack.h>
#include <common/trace.h>
#include <common/memory.h>
#include <engine/graphics/errors.h>
#include <engine/graphics/capability_tests.h>

const GUID DECLSPEC_SELECTANY IID_IDirectDrawSurface3	= { 0xDA044E00, 0x69B2, 0x11D0, { 0xA1, 0xD5, 0x00, 0xAA, 0x00, 0xB8, 0xDF, 0xBB } };
const GUID DECLSPEC_SELECTANY IID_IDirectDrawSurface4	= { 0x0B2B8630, 0xAD35, 0x11D0, { 0x8E, 0xA6, 0x00, 0x60, 0x97, 0x97, 0xEA, 0x5B } };
const GUID DECLSPEC_SELECTANY IID_IDirectDraw2		= { 0xB3A6F3E0, 0x2B43, 0x11CF, { 0xA2, 0xDE, 0x00, 0xAA, 0x00, 0xB9, 0x33, 0x56 } };
const GUID DECLSPEC_SELECTANY IID_IDirectDraw4		= { 0x9C59509A, 0x39BD, 0x11D1, { 0x8C, 0x4A, 0x00, 0xC0, 0x4F, 0xD9, 0x30, 0xC5 } };
const GUID DECLSPEC_SELECTANY IID_IDirect3D3		= { 0xBB223240, 0xE72B, 0x11D0, { 0xA9, 0xB4, 0x00, 0xAA, 0x00, 0xC0, 0x99, 0x3E } };

ZGraphics* ZGfx;
ZGraphicsExtra ZGfxEx;

void ZGfxInit(void) {
	ZGfx = (ZGraphics*)CHckVirtualAddressToActual(CHCK_DEFAULT_MODULE, 0x008026e0);

	ZGfxEx.directDrawCreate = *(ZGfxDirectDrawCreateProc*)CHckVirtualAddressToActual(CHCK_DEFAULT_MODULE, 0x005bb044);

	ZGfxEx.isFogEnabled	= (bool32_t*)CHckVirtualAddressToActual(CHCK_DEFAULT_MODULE, 0x008093b0);
	ZGfxEx.fogMode		= (uint32_t*)CHckVirtualAddressToActual(CHCK_DEFAULT_MODULE, 0x008093b8);

	ZGfxEx.driverGUID	= (GUID***)CHckVirtualAddressToActual(CHCK_DEFAULT_MODULE, 0x00809644);
	ZGfxEx.directDraw	= (IDirectDraw4**)CHckVirtualAddressToActual(CHCK_DEFAULT_MODULE, 0x00809358);
	ZGfxEx.direct3D		= (IDirect3D3**)CHckVirtualAddressToActual(CHCK_DEFAULT_MODULE, 0x0080936c);
	ZGfxEx.direct3Ddevice	= (IDirect3DDevice3**)CHckVirtualAddressToActual(CHCK_DEFAULT_MODULE, 0x00809370);

	ZGfxEx.hal = (ZGfxHal*)CHckVirtualAddressToActual(CHCK_DEFAULT_MODULE, 0x00809324);
}

HRESULT WINAPI ZGfxDirectDrawCreate(GUID FAR *lpGUID, LPDIRECTDRAW FAR *lplpDD, IUnknown FAR *pUnkOuter) {
	return ZGfxEx.directDrawCreate(lpGUID, lplpDD, pUnkOuter);
}

HRESULT __stdcall ZGfxInitDirectDraw(void) {
	IDirectDraw* directDraw;

	HRESULT result = ZGfxEx.directDrawCreate(**ZGfxEx.driverGUID, &directDraw, NULL);
	if (result != S_OK) {
		ZGFX_HANDLE_ERROR(result);
		return result;
	}

	result = directDraw->QueryInterface(IID_IDirectDraw4, (void**)ZGfxEx.directDraw);
	if (result != S_OK) {
		ZGFX_HANDLE_ERROR(result);

		directDraw->Release();
		return result;
	}

	directDraw->Release();
	return S_OK;
}

bool __fastcall ZGfxEnumerate3DDevices(ZGfxDevice* device) {
	CTRC_TRACE("Enumerating Devices (%s - %s)...", device->name, device->description);
	CMEM_DUMP(device);

	HRESULT res = (*ZGfxEx.directDraw)->QueryInterface(IID_IDirect3D3, (void**)ZGfxEx.direct3D);
	if (res != S_OK) {
		ZGFX_HANDLE_ERROR(res);
		return false;
	}

	device->suitableD3DDevicesCount = 0;
	(*ZGfxEx.direct3D)->EnumDevices(ZGfxCheckDeviceSuitability, device);
	_STATIC_ASSERT(sizeof(DDSURFACEDESC2) == 124);

	if (*ZGfxEx.direct3D != NULL) {
		(*ZGfxEx.direct3D)->Release();
		*ZGfxEx.direct3D = NULL;
	}

	if (device->suitableD3DDevicesCount == 0) {
		CTRC_TRACE("No suitable devices.");
		return false;
	}

	return true;
}


