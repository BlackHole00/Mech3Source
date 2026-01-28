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
	res = (*ZGfxEx.surface)->Blt(rect, NULL, NULL, DDBLT_DEPTHFILL, &blitDesc);

	if (res == DDERR_SURFACELOST) {
		res = (*ZGfxEx.surface)->Restore();

	}
	if (res != S_OK) {
		ZGFX_HANDLE_ERROR(res);
		return res;
	}

	return S_OK;
}
