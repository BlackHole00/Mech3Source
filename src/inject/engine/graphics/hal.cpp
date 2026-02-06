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
		res = ZGfx->zBufferSurface.ddSurface->Blt(rect, NULL, NULL, DDBLT_DEPTHFILL, &blitDesc);
		if (res == S_OK) {
			return S_OK;
		}

		if (res == DDERR_SURFACELOST) {
			res = ZGfx->zBufferSurface.ddSurface->Restore();
		}
	} while (res == S_OK);

	return ZGFX_HANDLE_ERROR(res);
}

void __stdcall ZGfxEvictTextures(void) {
	if (ZGfx->DAT_008026e0 != 1 || *ZGfxEx.direct3D == NULL) {
		return;
	}

	(*ZGfxEx.direct3D)->EvictManagedTextures();
}

bool __stdcall ZGfxEnterFullscreen(void) {
	HRESULT res;

	res = (*ZGfxEx.directDraw)->SetCooperativeLevel(ZGfx->windowHandle, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN | DDSCL_ALLOWREBOOT);
	if (res != S_OK) {
		ZGFX_HANDLE_ERROR(res);
		return false; 
	}

	res = (*ZGfxEx.directDraw)->SetDisplayMode(ZGfx->primarySurface.width, ZGfx->primarySurface.height, ZGfx->bpp, 0, 0);
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

	if ((ZGfx->drawMode & 1) != 0) {
		if (!ZGfx->isWireframeEnabled) {
			(*ZGfxEx.direct3Ddevice)->SetRenderState(D3DRENDERSTATE_FILLMODE, D3DFILL_SOLID);
		} else {
			(*ZGfxEx.direct3Ddevice)->SetRenderState(D3DRENDERSTATE_FILLMODE, D3DFILL_WIREFRAME);
		}
		ZGfx->drawMode &= 0xfffffffe;
	}
	if ((ZGfx->drawMode & 2) != 0) {
		(*ZGfxEx.direct3Ddevice)->SetRenderState(D3DRENDERSTATE_DITHERENABLE, ZGfx->isDitheringEnabled);
		ZGfx->drawMode &= 0xfffffffd;
	}

	return S_OK;
}

void __fastcall ZGfxSetFog(bool32_t enabled) {
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

void __fastcall ZGfxChangeResolution(ZGfxResolution resolution) {
	ZGfxSetupResolution(resolution);
	ZGfxEx.hal->applyResolution();
}

ZGfxError __fastcall ZGfxChangeResolutionS(ZGfxResolution resolution) {
	if (!ZGfx->isInitialized) {
		return ZGFX_NOT_INITIALIZED;
	}

	ZGfxSetupResolution(resolution);
	return ZGfxEx.hal->applyResolution();
}

void __fastcall ZGfxSetupResolution(ZGfxResolution resolution) {
	switch (resolution) {
		case ZGFX_UNKNOWN_RESOLUTION_0:
		case ZGFX_UNKNOWN_RESOLUTION_1: {
			ZGfx->DAT_00802714 = 0;
			ZGfx->bpp = 8;
		}
		case ZGFX_640X400_HALFDEPTH: {
			ZGfx->primaryAndDepthSurfaceHaveDifferentResolutions = true;
			ZGfx->primarySurface.width	= 640;
			ZGfx->primarySurface.height	= 400;
			ZGfx->attachedSurface.width	= 640;
			ZGfx->attachedSurface.height	= 400;
			ZGfx->zBufferSurface.width	= 320;
			ZGfx->zBufferSurface.height	= 200;
			ZGfx->bpp = 16;

			break;
		}
		case ZGFX_640X480_HALFDEPTH: {
			ZGfx->primaryAndDepthSurfaceHaveDifferentResolutions = true;
			ZGfx->primarySurface.width	= 640;
			ZGfx->primarySurface.height	= 480;
			ZGfx->attachedSurface.width	= 640;
			ZGfx->attachedSurface.height	= 480;
			ZGfx->zBufferSurface.width	= 320;
			ZGfx->zBufferSurface.height	= 240;
			ZGfx->bpp = 16;

			break;
		}
		case ZGFX_640X400: {
			ZGfx->primaryAndDepthSurfaceHaveDifferentResolutions = false;
			ZGfx->primarySurface.width	= 640;
			ZGfx->primarySurface.height	= 400;
			ZGfx->attachedSurface.width	= 640;
			ZGfx->attachedSurface.height	= 400;
			ZGfx->zBufferSurface.width	= 640;
			ZGfx->zBufferSurface.height	= 400;
			ZGfx->bpp = 16;

			break;
		}
		case ZGFX_640X480: {
			ZGfx->primaryAndDepthSurfaceHaveDifferentResolutions = false;
			ZGfx->primarySurface.width	= 640;
			ZGfx->primarySurface.height	= 480;
			ZGfx->attachedSurface.width	= 640;
			ZGfx->attachedSurface.height	= 480;
			ZGfx->zBufferSurface.width	= 640;
			ZGfx->zBufferSurface.height	= 480;
			ZGfx->bpp = 16;

			break;
		}
		case ZGFX_800X600: {
			ZGfx->primaryAndDepthSurfaceHaveDifferentResolutions = false;
			ZGfx->primarySurface.width	= 800;
			ZGfx->primarySurface.height	= 600;
			ZGfx->attachedSurface.width	= 800;
			ZGfx->attachedSurface.height	= 600;
			ZGfx->zBufferSurface.width	= 800;
			ZGfx->zBufferSurface.height	= 600;
			ZGfx->bpp = 16;

			break;
		}
		case ZGFX_1024X768: {
			ZGfx->primaryAndDepthSurfaceHaveDifferentResolutions = false;
			ZGfx->primarySurface.width	= 1024;
			ZGfx->primarySurface.height	= 768;
			ZGfx->attachedSurface.width	= 1024;
			ZGfx->attachedSurface.height	= 768;
			ZGfx->zBufferSurface.width	= 1024;
			ZGfx->zBufferSurface.height	= 768;
			ZGfx->bpp = 16;

			break;
		}
		case ZGFX_1152X864: {
			ZGfx->primaryAndDepthSurfaceHaveDifferentResolutions = false;
			ZGfx->primarySurface.width	= 1152;
			ZGfx->primarySurface.height	= 864;
			ZGfx->attachedSurface.width	= 1152;
			ZGfx->attachedSurface.height	= 864;
			ZGfx->zBufferSurface.width	= 1152;
			ZGfx->zBufferSurface.height	= 864;
			ZGfx->bpp = 16;

			break;
		}
		case ZGFX_1280X1024: {
			ZGfx->primaryAndDepthSurfaceHaveDifferentResolutions = false;
			ZGfx->primarySurface.width	= 1280;
			ZGfx->primarySurface.height	= 1024;
			ZGfx->attachedSurface.width	= 1280;
			ZGfx->attachedSurface.height	= 1024;
			ZGfx->zBufferSurface.width	= 1280;
			ZGfx->zBufferSurface.height	= 1024;
			ZGfx->bpp = 16;

			break;
		}
		case ZGFX_1600X1200: {
			ZGfx->primaryAndDepthSurfaceHaveDifferentResolutions = false;
			ZGfx->primarySurface.width	= 1600;
			ZGfx->primarySurface.height	= 1200;
			ZGfx->attachedSurface.width	= 1600;
			ZGfx->attachedSurface.height	= 1200;
			ZGfx->zBufferSurface.width	= 1600;
			ZGfx->zBufferSurface.height	= 1200;
			ZGfx->bpp = 16;

			break;
		}
		default: {
			ZGfx->DAT_00802714 = 0;
		}
	}

}
