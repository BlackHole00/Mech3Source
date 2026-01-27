#pragma once

#include <stdint.h>
#include <ddraw.h>
#include <d3d.h>

typedef HRESULT (WINAPI *ZGfxDirectDrawCreateProc)(GUID FAR *lpGUID, LPDIRECTDRAW FAR *lplpDD, IUnknown FAR *pUnkOuter);

typedef struct ZGfxD3DDEVICEDESCStorage {
	char deviceName[32];
	char deviceDescriptor[96];
	GUID* deviceGuidPtr;
	GUID deviceGuid;
	D3DDEVICEDESC descriptor;
} ZGfxD3DDEVICEDESCStorage;

typedef struct ZGraphics {
	char _unk0[0x14];
	uint32_t field_0x14;
	char _unk1[0xac - 0x14 - sizeof(uint32_t)];
	size_t suitableDevicesCount;
	ZGfxD3DDEVICEDESCStorage descriptors[3];
} ZGraphics;
extern ZGraphics ZGfx;

typedef struct {
	ZGfxDirectDrawCreateProc directDrawCreate;

	GUID*** driverGUID;
	IDirectDraw4** directDraw;
	IDirect3D3** direct3D;
} ZGraphicsExtra;
extern ZGraphicsExtra ZGfxEx;

void ZGfxInit(void);

HRESULT WINAPI ZGfxDirectDrawCreate(GUID FAR *lpGUID, LPDIRECTDRAW FAR *lplpDD, IUnknown FAR *pUnkOuter);

HRESULT __stdcall ZGfxInitDirectDraw(void);
bool __fastcall ZGfxEnumerateDevices(ZGraphics* gfx);

