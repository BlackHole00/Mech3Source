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
		res = ZGfxEx.zBufferSurface->ddSurface->Blt(rect, NULL, NULL, DDBLT_DEPTHFILL, &blitDesc);
		if (res == S_OK) {
			return S_OK;
		}

		if (res == DDERR_SURFACELOST) {
			res = ZGfxEx.zBufferSurface->ddSurface->Restore();
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

void __fastcall ZGfxSetupResolution(ZGfxResolution resolution) {
	switch (resolution) {
		case ZGFX_UNKNOWN_RESOLUTION_0:
		case ZGFX_UNKNOWN_RESOLUTION_1: {
			*ZGfxEx.DAT_00802714 = 0;
			*ZGfxEx.resolutionBpp = 8;
		}
		case ZGFX_640X400_HALFDEPTH: {
			*ZGfxEx.primaryAndDepthSurfaceHaveDifferentResolutions = true;
			ZGfxEx.primarySurface->width	= 640;
			ZGfxEx.primarySurface->height	= 400;
			ZGfxEx.attachedSurface->width	= 640;
			ZGfxEx.attachedSurface->height	= 400;
			ZGfxEx.zBufferSurface->width	= 320;
			ZGfxEx.zBufferSurface->height	= 200;
			*ZGfxEx.resolutionBpp = 16;

			break;
		}
		case ZGFX_640X480_HALFDEPTH: {
			*ZGfxEx.primaryAndDepthSurfaceHaveDifferentResolutions = true;
			ZGfxEx.primarySurface->width	= 640;
			ZGfxEx.primarySurface->height	= 480;
			ZGfxEx.attachedSurface->width	= 640;
			ZGfxEx.attachedSurface->height	= 480;
			ZGfxEx.zBufferSurface->width	= 320;
			ZGfxEx.zBufferSurface->height	= 240;
			*ZGfxEx.resolutionBpp = 16;

			break;
		}
		case ZGFX_640X400: {
			*ZGfxEx.primaryAndDepthSurfaceHaveDifferentResolutions = false;
			ZGfxEx.primarySurface->width	= 640;
			ZGfxEx.primarySurface->height	= 400;
			ZGfxEx.attachedSurface->width	= 640;
			ZGfxEx.attachedSurface->height	= 400;
			ZGfxEx.zBufferSurface->width	= 640;
			ZGfxEx.zBufferSurface->height	= 400;
			*ZGfxEx.resolutionBpp = 16;

			break;
		}
		case ZGFX_640X480: {
			*ZGfxEx.primaryAndDepthSurfaceHaveDifferentResolutions = false;
			ZGfxEx.primarySurface->width	= 640;
			ZGfxEx.primarySurface->height	= 480;
			ZGfxEx.attachedSurface->width	= 640;
			ZGfxEx.attachedSurface->height	= 480;
			ZGfxEx.zBufferSurface->width	= 640;
			ZGfxEx.zBufferSurface->height	= 480;
			*ZGfxEx.resolutionBpp = 16;

			break;
		}
		case ZGFX_800X600: {
			*ZGfxEx.primaryAndDepthSurfaceHaveDifferentResolutions = false;
			ZGfxEx.primarySurface->width	= 800;
			ZGfxEx.primarySurface->height	= 600;
			ZGfxEx.attachedSurface->width	= 800;
			ZGfxEx.attachedSurface->height	= 600;
			ZGfxEx.zBufferSurface->width	= 800;
			ZGfxEx.zBufferSurface->height	= 600;
			*ZGfxEx.resolutionBpp = 16;

			break;
		}
		case ZGFX_1024X768: {
			*ZGfxEx.primaryAndDepthSurfaceHaveDifferentResolutions = false;
			ZGfxEx.primarySurface->width	= 1024;
			ZGfxEx.primarySurface->height	= 768;
			ZGfxEx.attachedSurface->width	= 1024;
			ZGfxEx.attachedSurface->height	= 768;
			ZGfxEx.zBufferSurface->width	= 1024;
			ZGfxEx.zBufferSurface->height	= 768;
			*ZGfxEx.resolutionBpp = 16;

			break;
		}
		case ZGFX_1152X864: {
			*ZGfxEx.primaryAndDepthSurfaceHaveDifferentResolutions = false;
			ZGfxEx.primarySurface->width	= 1152;
			ZGfxEx.primarySurface->height	= 864;
			ZGfxEx.attachedSurface->width	= 1152;
			ZGfxEx.attachedSurface->height	= 864;
			ZGfxEx.zBufferSurface->width	= 1152;
			ZGfxEx.zBufferSurface->height	= 864;
			*ZGfxEx.resolutionBpp = 16;

			break;
		}
		case ZGFX_1280X1024: {
			*ZGfxEx.primaryAndDepthSurfaceHaveDifferentResolutions = false;
			ZGfxEx.primarySurface->width	= 1280;
			ZGfxEx.primarySurface->height	= 1024;
			ZGfxEx.attachedSurface->width	= 1280;
			ZGfxEx.attachedSurface->height	= 1024;
			ZGfxEx.zBufferSurface->width	= 1280;
			ZGfxEx.zBufferSurface->height	= 1024;
			*ZGfxEx.resolutionBpp = 16;

			break;
		}
		case ZGFX_1600X1200: {
			*ZGfxEx.primaryAndDepthSurfaceHaveDifferentResolutions = false;
			ZGfxEx.primarySurface->width	= 1600;
			ZGfxEx.primarySurface->height	= 1200;
			ZGfxEx.attachedSurface->width	= 1600;
			ZGfxEx.attachedSurface->height	= 1200;
			ZGfxEx.zBufferSurface->width	= 1600;
			ZGfxEx.zBufferSurface->height	= 1200;
			*ZGfxEx.resolutionBpp = 16;

			break;
		}
		default: {
			*ZGfxEx.DAT_00802714 = 0;
		}
	}

}
