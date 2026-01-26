#include <stdint.h>
#include <stdbool.h>

#include <common/hack.h>
#include <common/trace.h>
#include <common/utils.h>
#include <engine/graphics/graphics.h>
#include <engine/graphics/errors.h>
#include <engine/memory/memory.h>
#include <engine/localization/localization.h>
#include <engine/data/script_engine.h>
#include <engine/platform/platform.h>
#include <game/game.h>

// NOTE: It is not possible to refer to "Mech3.exe" as the main module, since the executable may be renamed (e.g. "Mech3Fixup.exe")
// 	Thus we must use CHCK_DEFAULT_MODULE (which equates to NULL) to get the module of the main executable.
// NOTE: mech3Msg.dll does not get loaded on startup. ZLocLoadMessagesDll is responsible to register its module.
CHckModuleDescriptor MECH3_MODULES[] = {
	{ CHCK_DEFAULT_MODULE, 	0x00400000 },
};

CHckCodeReplacement MECH3_REPLACEMENT_TABLE[] = {
	{ CHCK_DEFAULT_MODULE, 0x00575fd0, CHCK_VIRTUAL_ADDRESS, (void*)ZLocFormatString			},
	{ CHCK_DEFAULT_MODULE, 0x00575ff0, CHCK_VIRTUAL_ADDRESS, (void*)ZLocFormatStringV			},
	{ CHCK_DEFAULT_MODULE, 0x00575f90, CHCK_VIRTUAL_ADDRESS, (void*)ZLocGetId				},
	{ CHCK_DEFAULT_MODULE, 0x0051c5a0, CHCK_VIRTUAL_ADDRESS, (void*)ZLocGetString				},
	{ CHCK_DEFAULT_MODULE, 0x00576040, CHCK_VIRTUAL_ADDRESS, (void*)ZLocGetStringI				},
	{ CHCK_DEFAULT_MODULE, 0x00575f30, CHCK_VIRTUAL_ADDRESS, (void*)ZLocLoadMessagesDll			},
	{ CHCK_DEFAULT_MODULE, 0x00575f70, CHCK_VIRTUAL_ADDRESS, (void*)ZLocUnloadMessagesDll			},
	{ CHCK_DEFAULT_MODULE, 0x00575d30, CHCK_VIRTUAL_ADDRESS, (void*)ZPltGetTickCount			},
	{ CHCK_DEFAULT_MODULE, 0x004828d0, CHCK_VIRTUAL_ADDRESS, (void*)ZPltSetupCurrentWorkingDirectory	},
	{ CHCK_DEFAULT_MODULE, 0x0057a3d0, CHCK_VIRTUAL_ADDRESS, (void*)ZGfxCheckCapabilities			},
	{ CHCK_DEFAULT_MODULE, 0x00578cd0, CHCK_VIRTUAL_ADDRESS, (void*)ZGfxCheckDeviceSuitability		},
	{ CHCK_DEFAULT_MODULE, 0x00578c00, CHCK_VIRTUAL_ADDRESS, (void*)ZGfxEnumerateDevices			},
	{ CHCK_DEFAULT_MODULE, 0x00580550, CHCK_VIRTUAL_ADDRESS, (void*)ZGfxHandleError				},
	{ CHCK_DEFAULT_MODULE, 0x004af280, CHCK_VIRTUAL_ADDRESS, (void*)GGamGetVersionString			},
	{ CHCK_DEFAULT_MODULE, 0x004af285, CHCK_VIRTUAL_ADDRESS, (void*)GGamGetVersionString			},
	{ CHCK_DEFAULT_MODULE, 0x00595f30, CHCK_VIRTUAL_ADDRESS, CHckMethodPtrToPtr(&ZDatScriptEngine::GetScriptVariablePtr)	},
	{ CHCK_DEFAULT_MODULE, 0x00595fa0, CHCK_VIRTUAL_ADDRESS, CHckMethodPtrToPtr(&ZDatScriptEngine::SetVariableValue)	},
};

BOOL WINAPI DllMain(HINSTANCE h_instance, DWORD reason, LPVOID _reserved) {
	if (reason == DLL_PROCESS_ATTACH) {
		MessageBoxA(NULL, "If you need to attach a debugger, now it is a good time to do it!", "MechWarrior3 Source", MB_OK);

		CHckInit(MECH3_MODULES, CUTL_COUNTOF(MECH3_MODULES));

		CTrcInit();
		ZPltInit();
		ZMemInit();
		ZGfxInit();

		CHckApplyCodeReplacements(MECH3_REPLACEMENT_TABLE, CUTL_COUNTOF(MECH3_REPLACEMENT_TABLE));
	} else if (reason == DLL_PROCESS_DETACH) {
		CTrcDeinit();
	}

	return TRUE;
}
