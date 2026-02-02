#include "hal.h"

#include <engine/graphics/graphics.h>
#include <engine/graphics/errors.h>

HRESULT __fastcall ZGfxClearSurfaceZBuffer(RECT* rect) {
	if (*ZGfxEx.directDraw == NULL) {
		return S_OK;
	}

	DDBLTFX blitDesc;
	blitDesc.dwSize = sizeof(DDBLTFX);
	blitDesc.dwFillDepth = 0;

	HRESULT res;
	res = (*ZGfxEx.zBufferSurface)->Blt(rect, NULL, NULL, DDBLT_DEPTHFILL, &blitDesc);

	if (res == DDERR_SURFACELOST) {
		res = (*ZGfxEx.zBufferSurface)->Restore();

	}
	if (res != S_OK) {
		ZGFX_HANDLE_ERROR(res);
		return res;
	}

	return S_OK;
}

void __stdcall ZGfxEvictTextures(void) {
	if (*ZGfxEx.DAT_008026e0 != 1 || *ZGfxEx.direct3D == NULL) {
		return;
	}

	(*ZGfxEx.direct3D)->EvictManagedTextures();
}

bool __stdcall ZGfxEnterFullscreen(void) {
	HRESULT res;

	res = (*ZGfxEx.directDraw)->SetCooperativeLevel(*ZGfxEx.mainWindow, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN | DDSCL_ALLOWREBOOT);
	if (res != S_OK) {
		ZGFX_HANDLE_ERROR(res);
		return false; 
	}

	res = (*ZGfxEx.directDraw)->SetDisplayMode(*ZGfxEx.resolutionWidth, *ZGfxEx.resolutionHeight, *ZGfxEx.resolutionBpp, 0, 0);
	if (res != S_OK) {
		ZGFX_HANDLE_ERROR(res);
		return false; 
	}

	return true;
}

