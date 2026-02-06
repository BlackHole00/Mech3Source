#pragma once

#include <stdint.h>
#include <ddraw.h>
#include <d3d.h>

#include <common/types.h>
#include <engine/graphics/surface.h>
#include <engine/graphics/hal.h>

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

typedef struct ZGfxPixelFormat {
	uint32_t redChannelSize;
	uint32_t greenChannelSize;
	uint32_t blueChannelSize;

	uint32_t redBitMask;
	uint32_t greenBitMask;
	uint32_t blueBitMask;

	uint32_t redRightShift;
	uint32_t greenRightShift;
	uint32_t blueLeftShift;

	uint32_t redExpandMask;
	uint32_t greenExpandMask;
	uint32_t blueExpandMask;
} ZGfxPixelFormat;

typedef struct ZGraphics {
	undefined4_t DAT_008026e0;

	bool32_t isPrimarySurfaceModifiable;
	bool32_t primaryAndDepthSurfaceHaveDifferentResolutions;
	uint8_t _unknown0[0x8];
	bool32_t isPrimarySurfaceValid; // ?
	uint32_t drawMode; // see ZGfxBeginScene
	bool32_t isWireframeEnabled;
	bool32_t isDitheringEnabled;
	uint8_t _unknown1[0x10];

	undefined4_t DAT_00802714;
	uint32_t bpp;
	bool32_t isInitialized; // ?
	ZGfxPixelFormat primaryPixelFormat;
	uint8_t _unknown2[0x40];

	HWND windowHandle;
	uint8_t _unknown3[0x10];

	ZGfxSurface zBufferSurface;
	ZGfxSurface attachedSurface;
	ZGfxSurface primarySurface;

	// INCOMPLETE. The real size is 0x19e7bc. It's a BIIIIIIG BOY...
} ZGraphics;
// _STATIC_ASSERT(sizeof(ZGraphics) == 0x19e7bc);
_STATIC_ASSERT(offsetof(ZGraphics, isPrimarySurfaceValid) == 0x14);
_STATIC_ASSERT(offsetof(ZGraphics, DAT_00802714) == 0x34);
_STATIC_ASSERT(offsetof(ZGraphics, windowHandle) == 0xb0);
_STATIC_ASSERT(offsetof(ZGraphics, zBufferSurface) == 0x00c4);

extern ZGraphics* ZGfx;

typedef struct {
	ZGfxDirectDrawCreateProc directDrawCreate;

	GUID*** driverGUID;
	IDirectDraw4** directDraw;
	IDirect3D3** direct3D;
	IDirect3DDevice3** direct3Ddevice;

	bool32_t* isFogEnabled;
	uint32_t* fogMode;

	ZGfxHal* hal;
} ZGraphicsExtra;
extern ZGraphicsExtra ZGfxEx;

void ZGfxInit(void);

HRESULT WINAPI ZGfxDirectDrawCreate(GUID FAR *lpGUID, LPDIRECTDRAW FAR *lplpDD, IUnknown FAR *pUnkOuter);

HRESULT __stdcall ZGfxInitDirectDraw(void);
bool __fastcall ZGfxEnumerate3DDevices(ZGfxDevice* gfx);

