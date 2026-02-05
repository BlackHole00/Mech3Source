#include "surface.h"
#include "engine/graphics/graphics.h"

#include <common/memory.h>
#include <engine/graphics/errors.h>
#include <yvals_core.h>

HRESULT __fastcall ZGfxLockDDSurface(IDirectDrawSurface3* ddSurface, DDSURFACEDESC2* surfaceDesc) {
	CMEM_ZERO(*surfaceDesc);
	surfaceDesc->dwSize = sizeof(DDSURFACEDESC2);

	HRESULT res;
	do {
		res = ddSurface->Lock(NULL, (DDSURFACEDESC*)surfaceDesc, DDLOCK_WAIT, NULL);
		if (res == S_OK) {
			return S_OK;
		}

		if (res == DDERR_SURFACELOST) {
			res = ddSurface->Restore();
		}

		// NOTE: If res is S_OK, then the surface restoration was successfull and it is safe to retry lockinig the surface
	} while (res == S_OK);

	ZGFX_HANDLE_ERROR(res);
	return 0x5a56ffff;
}

HRESULT __fastcall ZGfxUnlockDDSurface(IDirectDrawSurface3* ddSurface) {
	HRESULT res;
	do {
		res = ddSurface->Unlock(NULL);
		if (res == S_OK) {
			return S_OK;
		}

		if (res == DDERR_SURFACELOST) {
			res = ddSurface->Restore();
		}

		// NOTE: If res is S_OK, then the surface restoration was successfull and it is safe to retry unlockinig the surface
	} while(res == S_OK);

	ZGFX_HANDLE_ERROR(res);
	return 0x5a56ffff;
}

HRESULT __fastcall ZGfxLockSurface(ZGfxSurface* surface) {
	if (surface == ZGfxEx.primarySurface && !*ZGfxEx.isPrimarySurfaceLocked) {
		return S_OK;
	}

	if (surface->isLocked) {
		return S_OK;
	}

	DDSURFACEDESC2 surfaceDesc;
	HRESULT res = ZGfxLockDDSurface(surface->ddSurface, &surfaceDesc);
	if (res == S_OK) {
		surface->width		= surfaceDesc.dwWidth;
		surface->height		= surfaceDesc.dwHeight;
		surface->isLocked	= true;
		surface->unk0		= 1;
		surface->pitch		= surfaceDesc.dwLinearSize;
		surface->surfaceMemory	= surfaceDesc.lpSurface;
	}

	return res;
}

HRESULT __fastcall ZGfxUnlockSurface(ZGfxSurface* surface) {
	if (surface == ZGfxEx.primarySurface && !*ZGfxEx.isPrimarySurfaceLocked) {
		return S_OK;
	}

	if (!surface->isLocked) {
		return S_OK;
	}

	surface->isLocked = false;
	HRESULT res = ZGfxUnlockDDSurface(surface->ddSurface);

	return res;
}
