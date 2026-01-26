#include "errors.h"

#include <stdio.h>
#include <assert.h>
#include <ddraw.h>
#include <d3d.h>

#include <common/trace.h>

uint32_t __fastcall ZGfxHandleError(HRESULT result, const char* sourceCodePath, uint32_t lineNumber) {
	const char* errorName;

	if (result == S_OK) {
		return 0;
	}

	switch (result) {
		case DDERR_GENERIC: {
			errorName = "DDERR_GENERIC";
			break;
		}
		case DDERR_UNSUPPORTED: {
			errorName = "DDERR_UNSUPPORTED";
			break;
		}
		case DDERR_OUTOFMEMORY: {
			errorName = "DDERR_OUTOFMEMORYA";
			break;
		}
		case DDERR_NOTINITIALIZED: {
			errorName = "DDERR_NOTINITIALIZED";
			break;
		}
		case DDERR_ALREADYINITIALIZED: {
			errorName = "DDERR_ALREADYINITIALIZED";
			break;
		}
		case DDERR_INVALIDPARAMS: {
			errorName = "DDERR_INVALIDPARAMS";
			break;
		}
		case DDERR_CANNOTDETACHSURFACE: {
			errorName = "DDERR_CANNOTDETACHSURFACE";
			break;
		}
		case DDERR_CANNOTATTACHSURFACE: {
			errorName = "DDERR_CANNOTATTACHSURFACE";
			break;
		}
		case DDERR_EXCEPTION: {
			errorName = "DDERR_EXCEPTION";
			break;
		}
		case DDERR_CURRENTLYNOTAVAIL: {
			errorName = "DDERR_CURRENTLYNOTAVAIL";
			break;
		}
		case DDERR_INVALIDCAPS: {
			errorName = "DDERR_INVALIDCAPS";
			break;
		}
		case DDERR_HEIGHTALIGN: {
			errorName = "DDERR_HEIGHTALIGN";
			break;
		}
		case DDERR_INVALIDMODE: {
			errorName = "DDERR_INVALIDMODE";
			break;
		}
		case DDERR_INVALIDCLIPLIST: {
			errorName = "DDERR_INVALIDCLIPLIST";
			break;
		}
		case DDERR_INVALIDPIXELFORMAT: {
			errorName = "DDERR_INVALIDPIXELFORMAT";
			break;
		}
		case DDERR_INVALIDOBJECT: {
			errorName = "DDERR_INVALIDOBJECT";
			break;
		}
		case DDERR_LOCKEDSURFACES: {
			errorName = "DDERR_LOCKEDSURFACES";
			break;
		}
		case DDERR_INVALIDRECT: {
			errorName = "DDERR_INVALIDRECT";
			break;
		}
		case DDERR_NOALPHAHW: {
			errorName = "DDERR_NOALPHAHW";
			break;
		}
		case DDERR_NO3D: {
			errorName = "DDERR_NO3D";
			break;
		}
		case DDERR_NOCOLORCONVHW: {
			errorName = "DDERR_NOCOLORCONVHW";
			break;
		}
		case DDERR_NOCLIPLIST: {
			errorName = "DDERR_NOCLIPLIST";
			break;
		}
		case DDERR_NOCOLORKEY: {
			errorName = "DDERR_NOCOLORKEY";
			break;
		}
		case DDERR_NOCOOPERATIVELEVELSET: {
			errorName = "DDERR_NOCOOPERATIVELEVELSET";
			break;
		}
		case DDERR_NOFLIPHW: {
			errorName = "DDERR_NOFLIPHW";
			break;
		}
		case DDERR_NOCOLORKEYHW: {
			errorName = "DDERR_NOCOLORKEYHW";
			break;
		}
		case DDERR_NODIRECTDRAWSUPPORT: {
			errorName = "DDERR_NODIRECTDRAWSUPPORT";
			break;
		}
		case DDERR_NOEXCLUSIVEMODE: {
			errorName = "DDERR_NOEXCLUSIVEMODE";
			break;
		}
		case DDERR_NOMIRRORHW: {
			errorName = "DDERR_NOMIRRORHW";
			break;
		}
		case DDERR_NOGDI: {
			errorName = "DDERR_NOGDI";
			break;
		}
		case DDERR_NOOVERLAYHW: {
			errorName = "DDERR_NOOVERLAYHW";
			break;
		}
		case DDERR_NOTFOUND: {
			errorName = "DDERR_NOTFOUND";
			break;
		}
		case DDERR_NOROTATIONHW: {
			errorName = "DDERR_NOROTATIONHW";
			break;
		}
		case DDERR_NORASTEROPHW: {
			errorName = "DDERR_NORASTEROPHW";
			break;
		}
		case DDERR_NOT8BITCOLOR: {
			errorName = "DDERR_NOT8BITCOLOR";
			break;
		}
		case DDERR_NOT4BITCOLORINDEX: {
			errorName = "DDERR_NOT4BITCOLORINDEX";
			break;
		}
		case DDERR_NOVSYNCHW: {
			errorName = "DDERR_NOVSYNCHW";
			break;
		}
		case DDERR_NOTEXTUREHW: {
			errorName = "DDERR_NOTEXTUREHW";
			break;
		}
		case DDERR_NOZOVERLAYHW: {
			errorName = "DDERR_NOZOVERLAYHW";
			break;
		}
		case DDERR_OUTOFVIDEOMEMORY: {
			errorName = "DDERR_OUTOFVIDEOMEMORY";
			break;
		}
		case DDERR_OUTOFCAPS: {
			errorName = "DDERR_OUTOFCAPS";
			break;
		}
		case DDERR_COLORKEYNOTSET: {
			errorName = "DDERR_COLORKEYNOTSET";
			break;
		}
		case DDERR_OVERLAYCANTCLIP: {
			errorName = "DDERR_OVERLAYCANTCLIP";
			break;
		}
		case DDERR_OVERLAYCOLORKEYONLYONEACTIVE: {
			errorName = "DDERR_OVERLAYCOLORKEYONLYONEACTIVE";
			break;
		}
		case DDERR_PALETTEBUSY: {
			errorName = "DDERR_PALETTEBUSY";
			break;
		}
		case DDERR_SURFACEALREADYDEPENDENT: {
			errorName = "DDERR_SURFACEALREADYDEPENDENT";
			break;
		}
		case DDERR_SURFACEALREADYATTACHED: {
			errorName = "DDERR_SURFACEALREADYATTACHED";
			break;
		}
		case DDERR_CANTLOCKSURFACE: {
			errorName = "DDERR_CANTLOCKSURFACE";
			break;
		}
		case DDERR_SURFACEBUSY: {
			errorName = "DDERR_SURFACEBUSY";
			break;
		}
		case DDERR_SURFACEISOBSCURED: {
			errorName = "DDERR_SURFACEISOBSCURED";
			break;
		}
		case DDERR_TOOBIGHEIGHT: {
			errorName = "DDERR_TOOBIGHEIGHT";
			break;
		}
		case DDERR_SURFACENOTATTACHED: {
			errorName = "DDERR_SURFACENOTATTACHED";
			break;
		}
		case DDERR_TOOBIGWIDTH: {
			errorName = "DDERR_TOOBIGWIDTH";
			break;
		}
		case DDERR_TOOBIGSIZE: {
			errorName = "DDERR_TOOBIGSIZE";
			break;
		}
		case DDERR_UNSUPPORTEDMASK: {
			errorName = "DDERR_UNSUPPORTEDMASK";
			break;
		}
		case DDERR_UNSUPPORTEDFORMAT: {
			errorName = "DDERR_UNSUPPORTEDFORMAT";
			break;
		}
		case DDERR_WASSTILLDRAWING: {
			errorName = "DDERR_WASSTILLDRAWING";
			break;
		}
		case DDERR_VERTICALBLANKINPROGRESS: {
			errorName = "DDERR_VERTICALBLANKINPROGRESS";
			break;
		}
		case DDERR_CANTPAGELOCK: {
			errorName = "DDERR_CANTPAGELOCK";
			break;
		}
		case DDERR_XALIGN: {
			errorName = "DDERR_XALIGN";
			break;
		}
		case DDERR_INVALIDDIRECTDRAWGUID: {
			errorName = "DDERR_INVALIDDIRECTDRAWGUID";
			break;
		}
		case DDERR_DIRECTDRAWALREADYCREATED: {
			errorName = "DDERR_DIRECTDRAWALREADYCREATED";
			break;
		}
		case DDERR_NODIRECTDRAWHW: {
			errorName = "DDERR_NODIRECTDRAWHW";
			break;
		}
		case DDERR_PRIMARYSURFACEALREADYEXISTS: {
			errorName = "DDERR_PRIMARYSURFACEALREADYEXISTS";
			break;
		}
		case DDERR_NOEMULATION: {
			errorName = "DDERR_NOEMULATION";
			break;
		}
		case DDERR_REGIONTOOSMALL: {
			errorName = "DDERR_REGIONTOOSMALL";
			break;
		}
		case DDERR_CLIPPERISUSINGHWND: {
			errorName = "DDERR_CLIPPERISUSINGHWND";
			break;
		}
		case DDERR_NOCLIPPERATTACHED: {
			errorName = "DDERR_NOCLIPPERATTACHED";
			break;
		}
		case DDERR_NOHWND: {
			errorName = "DDERR_NOHWND";
			break;
		}
		case DDERR_HWNDSUBCLASSED: {
			errorName = "DDERR_HWNDSUBCLASSED";
			break;
		}
		case DDERR_HWNDALREADYSET: {
			errorName = "DDERR_HWNDALREADYSET";
			break;
		}
		case DDERR_NOPALETTEATTACHED: {
			errorName = "DDERR_NOPALETTEATTACHED";
			break;
		}
		case DDERR_NOPALETTEHW: {
			errorName = "DDERR_NOPALETTEHW";
			break;
		}
		case DDERR_BLTFASTCANTCLIP: {
			errorName = "DDERR_BLTFASTCANTCLIP";
			break;
		}
		case DDERR_NOBLTHW: {
			errorName = "DDERR_NOBLTHW";
			break;
		}
		case DDERR_NODDROPSHW: {
			errorName = "DDERR_NODDROPSHW";
			break;
		}
		case DDERR_OVERLAYNOTVISIBLE: {
			errorName = "DDERR_OVERLAYNOTVISIBLE";
			break;
		}
		case DDERR_INVALIDPOSITION: {
			errorName = "DDERR_INVALIDPOSITION";
			break;
		}
		case DDERR_NOTAOVERLAYSURFACE: {
			errorName = "DDERR_NOTAOVERLAYSURFACE";
			break;
		}
		case DDERR_EXCLUSIVEMODEALREADYSET: {
			errorName = "DDERR_EXCLUSIVEMODEALREADYSET";
			break;
		}
		case DDERR_NOTFLIPPABLE: {
			errorName = "DDERR_NOTFLIPPABLE";
			break;
		}
		case DDERR_CANTDUPLICATE: {
			errorName = "DDERR_CANTDUPLICATE";
			break;
		}
		case DDERR_NOTLOCKED: {
			errorName = "DDERR_NOTLOCKED";
			break;
		}
		case DDERR_CANTCREATEDC: {
			errorName = "DDERR_CANTCREATEDC";
			break;
		}
		case (HRESULT)0x8876024a: {
			errorName = "DDERR_NODIRECTDC";
			break;
		}
		case DDERR_WRONGMODE: {
			errorName = "DDERR_WRONGMODE";
			break;
		}
		case DDERR_IMPLICITLYCREATED: {
			errorName = "DDERR_IMPLICITLYCREATED";
			break;
		}
		case DDERR_NOTPALETTIZED: {
			errorName = "DDERR_NOTPALETTIZED";
			break;
		}
		case DDERR_UNSUPPORTEDMODE: {
			errorName = "DDERR_UNSUPPORTEDMODE";
			break;
		}
		case DDERR_NOMIPMAPHW: {
			errorName = "DDERR_NOMIPMAPHW";
			break;
		}
		case DDERR_INVALIDSURFACETYPE: {
			errorName = "DDERR_INVALIDSURFACETYPE";
			break;
		}
		case DDERR_DCALREADYCREATED: {
			errorName = "DDERR_DCALREADYCREATED";
			break;
		}
		case DDERR_NOTPAGELOCKED: {
			errorName = "DDERR_NOTPAGELOCKED";
			break;
		}
		case DDERR_CANTPAGEUNLOCK: {
			errorName = "DDERR_CANTPAGEUNLOCK";
			break;
		}
		case D3DERR_INVALID_DEVICE: {
			errorName = "D3DERR_INVALID_DEVICE";
			break;
		}
		case D3DERR_BADMAJORVERSION: {
			errorName = "D3DERR_BADMAJORVERSION";
			break;
		}
		case D3DERR_BADMINORVERSION: {
			errorName = "D3DERR_BADMINORVERSION";
			break;
		}
		case D3DERR_LIGHT_SET_FAILED: {
			errorName = "D3DERR_LIGHT_SET_FAILED";
			break;
		}
		case D3DERR_EXECUTE_CREATE_FAILED: {
			errorName = "D3DERR_EXECUTE_CREATE_FAILED";
			break;
		}
		case D3DERR_EXECUTE_DESTROY_FAILED: {
			errorName = "D3DERR_EXECUTE_DESTROY_FAILED";
			break;
		}
		case D3DERR_EXECUTE_LOCK_FAILED: {
			errorName = "D3DERR_EXECUTE_LOCK_FAILED";
			break;
		}
		case D3DERR_EXECUTE_UNLOCK_FAILED: {
			errorName = "D3DERR_EXECUTE_UNLOCK_FAILED";
			break;
		}
		case D3DERR_EXECUTE_LOCKED: {
			errorName = "D3DERR_EXECUTE_LOCKED";
			break;
		}
		case D3DERR_EXECUTE_NOT_LOCKED: {
			errorName = "D3DERR_EXECUTE_NOT_LOCKED";
			break;
		}
		case D3DERR_EXECUTE_FAILED: {
			errorName = "D3DERR_EXECUTE_FAILED";
			break;
		}
		case D3DERR_EXECUTE_CLIPPED_FAILED: {
			errorName = "D3DERR_EXECUTE_CLIPPED_FAILED";
			break;
		}
		case D3DERR_TEXTURE_NO_SUPPORT: {
			errorName = "D3DERR_TEXTURE_NO_SUPPORT";
			break;
		}
		case D3DERR_TEXTURE_CREATE_FAILED: {
			errorName = "D3DERR_TEXTURE_CREATE_FAILED";
			break;
		}
		case D3DERR_TEXTURE_DESTROY_FAILED: {
			errorName = "D3DERR_TEXTURE_DESTROY_FAILED";
			break;
		}
		case D3DERR_TEXTURE_LOCK_FAILED: {
			errorName = "D3DERR_TEXTURE_LOCK_FAILED";
			break;
		}
		case D3DERR_TEXTURE_SWAP_FAILED: {
			errorName = "D3DERR_TEXTURE_SWAP_FAILED";
			break;
		}
		case D3DERR_TEXTURE_LOCKED: {
			errorName = "D3DERR_TEXTURE_LOCKED";
			break;
		}
		case D3DERR_TEXTURE_NOT_LOCKED: {
			errorName = "D3DERR_TEXTURE_NOT_LOCKED";
			break;
		}
		case D3DERR_TEXTURE_GETSURF_FAILED: {
			errorName = "D3DERR_TEXTURE_GETSURF_FAILED";
			break;
		}
		case D3DERR_MATRIX_CREATE_FAILED: {
			errorName = "D3DERR_MATRIX_CREATE_FAILED";
			break;
		}
		case D3DERR_MATRIX_DESTROY_FAILED: {
			errorName = "D3DERR_MATRIX_DESTROY_FAILED";
			break;
		}
		case D3DERR_MATRIX_SETDATA_FAILED: {
			errorName = "D3DERR_MATRIX_SETDATA_FAILED";
			break;
		}
		case D3DERR_MATRIX_GETDATA_FAILED: {
			errorName = "D3DERR_MATRIX_GETDATA_FAILED";
			break;
		}
		case D3DERR_SETVIEWPORTDATA_FAILED: {
			errorName = "D3DERR_SETVIEWPORTDATA_FAILED";
			break;
		}
		case D3DERR_INVALIDCURRENTVIEWPORT: {
			errorName = "D3DERR_INVALIDCURRENTVIEWPORT";
			break;
		}
		case D3DERR_INVALIDPRIMITIVETYPE: {
			errorName = "D3DERR_INVALIDPRIMITIVETYPE";
			break;
		}
		case D3DERR_INVALIDVERTEXTYPE: {
			errorName = "D3DERR_INVALIDVERTEXTYPE";
			break;
		}
		case D3DERR_TEXTURE_BADSIZE: {
			errorName = "D3DERR_TEXTURE_BADSIZE";
			break;
		}
		case D3DERR_MATERIAL_CREATE_FAILED: {
			errorName = "D3DERR_MATERIAL_CREATE_FAILED";
			break;
		}
		case D3DERR_MATERIAL_DESTROY_FAILED: {
			errorName = "D3DERR_MATERIAL_DESTROY_FAILED";
			break;
		}
		case D3DERR_MATERIAL_SETDATA_FAILED: {
			errorName = "D3DERR_MATERIAL_SETDATA_FAILED";
			break;
		}
		case D3DERR_MATERIAL_GETDATA_FAILED: {
			errorName = "D3DERR_MATERIAL_GETDATA_FAILED";
			break;
		}
		case D3DERR_ZBUFF_NEEDS_SYSTEMMEMORY: {
			errorName = "D3DERR_ZBUFF_NEEDS_SYSTEMMEMORY";
			break;
		}
		case D3DERR_ZBUFF_NEEDS_VIDEOMEMORY: {
			errorName = "D3DERR_ZBUFF_NEEDS_VIDEOMEMORY";
			break;
		}
		case D3DERR_SCENE_IN_SCENE: {
			errorName = "D3DERR_SCENE_IN_SCENE";
			break;
		}
		case D3DERR_SCENE_NOT_IN_SCENE: {
			errorName = "D3DERR_SCENE_NOT_IN_SCENE";
			break;
		}
		case D3DERR_SCENE_BEGIN_FAILED: {
			errorName = "D3DERR_SCENE_BEGIN_FAILED";
			break;
		}
		case D3DERR_SCENE_END_FAILED: {
			errorName = "D3DERR_SCENE_END_FAILED";
			break;
		}
		case D3DERR_INBEGIN: {
			errorName = "D3DERR_INBEGIN";
			break;
		}
		case D3DERR_NOTINBEGIN: {
			errorName = "D3DERR_NOTINBEGIN";
			break;
		}
		case D3DERR_NOVIEWPORTS: {
			errorName = "D3DERR_NOVIEWPORTS";
			break;
		}
		case D3DERR_VIEWPORTDATANOTSET: {
			errorName = "D3DERR_VIEWPORTDATANOTSET";
			break;
		}
		default: {
			errorName = "Unknown Error";
		}
	}

	char errorNameBuffer[252];
	strncpy(errorNameBuffer, errorName, 252);

	if (result == (HRESULT)0x8876017c) {
		// TODO: Hook functions at 0x00581023
	}

	char buffer[256];
	snprintf(buffer, 256, "DirectDraw Error [%s]", errorNameBuffer);

	CTrcTrace("[%s:%d] - %s.\n", sourceCodePath, lineNumber, buffer);
	assert(false && "Encountered DirectDraw Error.");

	return 0xFFFFFFFF;
}

