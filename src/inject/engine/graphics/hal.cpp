#include "hal.h"

#include <common/trace.h>
#include <engine/platform/platform.h>
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
	do {
		res = (*ZGfxEx.zBufferSurface)->Blt(rect, NULL, NULL, DDBLT_DEPTHFILL, &blitDesc);
		if (res == S_OK) {
			return S_OK;
		}

		if (res == DDERR_SURFACELOST) {
			res = (*ZGfxEx.zBufferSurface)->Restore();
		}
	} while (res == S_OK);

	return ZGFX_HANDLE_ERROR(res);
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

HRESULT __stdcall ZGfxBeginScene(void) {
	HRESULT res;

	res = (*ZGfxEx.direct3Ddevice)->BeginScene();
	if (res != S_OK) {
		ZGFX_HANDLE_ERROR(res);
		return res;
	}

	if ((*ZGfxEx.drawMode & 1) != 0) {
		if (!*ZGfxEx.isWireframeEnabled) {
			(*ZGfxEx.direct3Ddevice)->SetRenderState(D3DRENDERSTATE_FILLMODE, D3DFILL_SOLID);
		} else {
			(*ZGfxEx.direct3Ddevice)->SetRenderState(D3DRENDERSTATE_FILLMODE, D3DFILL_WIREFRAME);
		}
		*ZGfxEx.drawMode &= 0xfffffffe;
	}
	if ((*ZGfxEx.drawMode & 2) != 0) {
		(*ZGfxEx.direct3Ddevice)->SetRenderState(D3DRENDERSTATE_DITHERENABLE, *ZGfxEx.isDitheringEnabled);
		*ZGfxEx.drawMode &= 0xfffffffd;
	}

	return S_OK;
}

void __fastcall ZGfxSetFog(bool enabled) {
	if (*ZGfxEx.isFogEnabled != enabled) {
		(*ZGfxEx.direct3Ddevice)->SetRenderState(D3DRENDERSTATE_FOGENABLE, enabled);
		*ZGfxEx.isFogEnabled = enabled;
	}

	if (*ZGfxEx.fogMode != D3DFOG_LINEAR) {
		(*ZGfxEx.direct3Ddevice)->SetLightState(D3DLIGHTSTATE_FOGMODE, D3DFOG_LINEAR);
		*ZGfxEx.fogMode = D3DFOG_LINEAR;
	}
}

void __stdcall ZGfxSetFogColor(void) {
	int r = (int)(ZPlt.floatArgs[0] + 0.5);
	int g = (int)(ZPlt.floatArgs[1] + 0.5);
	int b = (int)(ZPlt.floatArgs[2] + 0.5);

	(*ZGfxEx.direct3Ddevice)->SetRenderState(D3DRENDERSTATE_FOGCOLOR, r << 16 | g << 8 | b);
}

