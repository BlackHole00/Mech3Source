#include <stdint.h>
#include <stdbool.h>

#include "game.h"
#include "graphics.h"
#include "hack.h"
#include "memory.h"
#include "platform.h"
#include "trace.h"
#include "localization.h"
#include "dat_script_engine.h"
#include "utils.h"

// NOTE: It is not possible to refer to "Mech3.exe" as the main module, since the executable may be renamed (e.g. "Mech3Fixup.exe")
// 	Thus we must use ZHCK_DEFAULT_MODULE (which equates to NULL) to get the module of the main executable.
// NOTE: mech3Msg.dll does not get loaded on startup. ZLocLoadMessagesDll is responsible to register its module.
ZHckModuleDescriptor MECH3_MODULES[] = {
	{ ZHCK_DEFAULT_MODULE, 	0x00400000 },
};

ZHckCodeReplacement MECH3_REPLACEMENT_TABLE[] = {
	{ ZHCK_DEFAULT_MODULE, 0x00575fd0, ZHCK_VIRTUAL_ADDRESS, (void*)ZLocFormatString			},
	{ ZHCK_DEFAULT_MODULE, 0x00575ff0, ZHCK_VIRTUAL_ADDRESS, (void*)ZLocFormatStringV			},
	{ ZHCK_DEFAULT_MODULE, 0x00575f90, ZHCK_VIRTUAL_ADDRESS, (void*)ZLocGetId				},
	{ ZHCK_DEFAULT_MODULE, 0x0051c5a0, ZHCK_VIRTUAL_ADDRESS, (void*)ZLocGetString				},
	{ ZHCK_DEFAULT_MODULE, 0x00576040, ZHCK_VIRTUAL_ADDRESS, (void*)ZLocGetStringI				},
	{ ZHCK_DEFAULT_MODULE, 0x00575f30, ZHCK_VIRTUAL_ADDRESS, (void*)ZLocLoadMessagesDll			},
	{ ZHCK_DEFAULT_MODULE, 0x00575f70, ZHCK_VIRTUAL_ADDRESS, (void*)ZLocUnloadMessagesDll			},
	{ ZHCK_DEFAULT_MODULE, 0x00575d30, ZHCK_VIRTUAL_ADDRESS, (void*)ZPltGetTickCount			},
	{ ZHCK_DEFAULT_MODULE, 0x004828d0, ZHCK_VIRTUAL_ADDRESS, (void*)ZPltSetupCurrentWorkingDirectory	},
	{ ZHCK_DEFAULT_MODULE, 0x0057a3d0, ZHCK_VIRTUAL_ADDRESS, (void*)ZGfxCheckCapabilities			},
	{ ZHCK_DEFAULT_MODULE, 0x00578cd0, ZHCK_VIRTUAL_ADDRESS, (void*)ZGfxCheckDeviceSuitability		},
	{ ZHCK_DEFAULT_MODULE, 0x004af280, ZHCK_VIRTUAL_ADDRESS, (void*)ZGamGetVersionString			},
	{ ZHCK_DEFAULT_MODULE, 0x004af285, ZHCK_VIRTUAL_ADDRESS, (void*)ZGamGetVersionString			},
	{ ZHCK_DEFAULT_MODULE, 0x00595f30, ZHCK_VIRTUAL_ADDRESS, ZHckMethodPtrToPtr(&ZDatScriptEngine::GetScriptVariablePtr)	},
	{ ZHCK_DEFAULT_MODULE, 0x00595fa0, ZHCK_VIRTUAL_ADDRESS, ZHckMethodPtrToPtr(&ZDatScriptEngine::SetVariableValue)	},
};

BOOL WINAPI DllMain(HINSTANCE h_instance, DWORD reason, LPVOID _reserved) {
	if (reason == DLL_PROCESS_ATTACH) {
		MessageBoxA(NULL, "If you need to attach a debugger, now it is a good time to do it!", "MechWarrior3 Source", MB_OK);

		ZHckInit(MECH3_MODULES, ZUTL_COUNTOF(MECH3_MODULES));

		ZTrcInit();
		ZPltInit();
		ZMemInit();

		ZHckApplyCodeReplacements(MECH3_REPLACEMENT_TABLE, ZUTL_COUNTOF(MECH3_REPLACEMENT_TABLE));
	} else if (reason == DLL_PROCESS_DETACH) {
		ZTrcDeinit();
	}

	return TRUE;
}
