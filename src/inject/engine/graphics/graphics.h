#pragma once

#include <stdint.h>
#include <ddraw.h>
#include <d3d.h>

#include <engine/graphics/surface.h>

typedef HRESULT (WINAPI *ZGfxDirectDrawCreateProc)(GUID FAR *lpGUID, LPDIRECTDRAW FAR *lplpDD, IUnknown FAR *pUnkOuter);

typedef struct ZGfx3DDevice {
	char name[32];
	char description[96];
	GUID* guidPtr;
	GUID guid;
	D3DDEVICEDESC descriptor;
} ZGfx3DDevice;

typedef struct ZGfxDevice {
	GUID guid;
	uint32_t _unk0;
	char description[32];
	char name[96];
	char _unk1[0xac - sizeof(GUID) - sizeof(uint32_t) - sizeof(char[32]) - sizeof(char[96])];
	size_t suitableD3DDevicesCount;
	ZGfx3DDevice d3DDevices[3];
} ZGfxDevice;

typedef struct ZGfxDeviceRecord {
	uint32_t unk0;
	ZGfxDevice entry;
	uint8_t padding[0xb0c - sizeof(ZGfxDevice) - sizeof(uint32_t)];
} ZGfxDeviceRecord;
_STATIC_ASSERT(sizeof(ZGfxDeviceRecord) == 0xb0c);

// VA: 0x00809644
// Could have data before that address;
typedef struct ZGfxDeviceManager {
	ZGfxDevice* selectedDevicePtr;
	ZGfxDeviceRecord records[4];
} ZGfxDeviceManager;

// typedef struct ZGfxDeviceManager {
// 	ZGfxDevicesContext* selectedDevice;
// 	ZGfxDevicesContext availableDevices[3];
// 	// uint32_t _pad[0x2c3];
// } ZGfxDeviceManager;
// _STATIC_ASSERT(sizeof(ZGfxDeviceManager) == 0x2c3 * 4);

typedef struct ZGraphics {
	uint8_t _pad0[0xc4];
	ZGfxSurface zBufferSurface;
	ZGfxSurface attachedSurface;
	ZGfxSurface primarySurface;
	uint8_t _pad1[0x6f64 - 0xc4 - 3 * sizeof(ZGfxSurface)];
	ZGfxDevice* selectedDevice;
	ZGfxDevice availableDevices[3];
	uint8_t _pad2[0x19e7bc - 0xc4 - 3 * sizeof(ZGfxSurface) - (0x6f64 - 0xc4 - 3 * sizeof(ZGfxSurface)) - (3 * sizeof(ZGfxDevice) + 4)];
} ZGraphics;
_STATIC_ASSERT(sizeof(ZGraphics) == 0x19e7bc);
_STATIC_ASSERT(offsetof(ZGraphics, zBufferSurface	) == 0x00c4);
_STATIC_ASSERT(offsetof(ZGraphics, attachedSurface	) == 0x00e8);
_STATIC_ASSERT(offsetof(ZGraphics, primarySurface	) == 0x010c);

extern ZGraphics* ZGfx;

typedef struct {
	ZGfxDirectDrawCreateProc directDrawCreate;

	// TODO: Move to Plt
	HWND* mainWindow;

	GUID*** driverGUID;
	IDirectDraw4** directDraw;
	IDirect3D3** direct3D;
	IDirect3DDevice3** direct3Ddevice;

	uint32_t* drawMode;
	bool* isWireframeEnabled;
	bool* isDitheringEnabled;
	bool* isFogEnabled;
	DWORD* fogMode;
	uint32_t* DAT_00802714;
	uint32_t* DAT_008026e0;
	uint32_t* resolutionWidth;
	uint32_t* resolutionHeight;
	uint32_t* resolutionBpp;
	// IDirectDrawSurface3** zBufferSurface;

	bool* primaryAndDepthSurfaceHaveDifferentResolutions;
	bool* isPrimarySurfaceModifiable;
	ZGfxSurface* primarySurface;
	ZGfxSurface* attachedSurface;
	ZGfxSurface* zBufferSurface;
} ZGraphicsExtra;
extern ZGraphicsExtra ZGfxEx;

void ZGfxInit(void);

HRESULT WINAPI ZGfxDirectDrawCreate(GUID FAR *lpGUID, LPDIRECTDRAW FAR *lplpDD, IUnknown FAR *pUnkOuter);

HRESULT __stdcall ZGfxInitDirectDraw(void);
bool __fastcall ZGfxEnumerate3DDevices(ZGfxDevice* gfx);

