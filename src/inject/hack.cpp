#include "hack.h"

#include <assert.h>
#include <cstdint>

ZHack ZHck;

void ZHckInit(const ZHckModuleDescriptor* modules, size_t modulesCount) {
	assert(modulesCount <= ZHCK_MAX_MODULES && "Too many modules specified.");

	ZHck.modulesCount = modulesCount;
	for (size_t i = 0; i < modulesCount; i++) {
		const ZHckModuleDescriptor* descriptor = &modules[i];
		ZHckRegisterModule(descriptor);
	}
}

void ZHckRegisterModule(const ZHckModuleDescriptor* descriptor) {
	assert(ZHck.modulesCount < ZHCK_MAX_MODULES && "Too many modules specified.");

	ZHckModule* module = &ZHck.modules[ZHck.modulesCount];

	module->name = descriptor->name;
	module->module = GetModuleHandleA(descriptor->name);
	module->imageBase = (uintptr_t)descriptor->imageBase;
	module->runtimeBase = (uintptr_t)module->module;

	assert(module->module != NULL && "Trying to register an invalid module.");

	ZHck.modulesCount++;
}

ZHckModule* ZHckGetModule(const char* name) {
	for (size_t i = 0; i < ZHck.modulesCount; i++) {
		ZHckModule* module = &ZHck.modules[i];

		if ((name == NULL && module->name != NULL) || (name != NULL && module->name == NULL)) {
			continue;
		}

		if ((module->name == NULL && name == NULL) || strcmp(module->name, name) == 0) {
			return module;
		}
	}

	return NULL;
}

void ZHckApplyCodeReplacement(const ZHckCodeReplacement* replacement) {
	void* targetAddress;
	switch (replacement->targetAddressType) {
	case ZHCK_VIRTUAL_ADDRESS: {
			targetAddress = (void*)ZHckVirtualAddressToActual(replacement->moduleName, replacement->targetCodeAddress);
			break;
		}
	case ZHCK_RELATIVE_VIRTUAL_ADDRESS: {
			targetAddress = (void*)ZHckRelativeVirtualAddressToActual(replacement->moduleName, replacement->targetCodeAddress);
			break;
		}
	case ZHCK_ACTUAL_ADDRESS: {
			targetAddress = (void*)replacement->targetCodeAddress;
			break;
		}
	}

	assert(targetAddress != NULL && "Invalid code replacement: trying to replace NULL. Is the specified module correct?");

	uint8_t patch[5];
	intptr_t rel = (uint8_t*)replacement->replacementFunction - ((uint8_t*)targetAddress + 5);
	patch[0] = 0xE9;
	*(int32_t*)(patch + 1) = (int32_t)rel;
	
	ZHckReplaceBytes(targetAddress, patch, 5, true);
}

void ZHckApplyCodeReplacements(const ZHckCodeReplacement* replacements, size_t replacementCount) {
	for (size_t i = 0; i < replacementCount; i++) {
		ZHckApplyCodeReplacement(&replacements[i]);
	}
}

void ZHckReplaceBytes(void* target, const void* source, size_t length, bool didReplaceCode) {
	DWORD oldProtect;

	VirtualProtect(target, length, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy(target, source, length);
	VirtualProtect(target, length, oldProtect, &oldProtect);

	if (didReplaceCode) {
		FlushInstructionCache(GetCurrentProcess(), target, length);
	}
}

uintptr_t ZHckVirtualAddressToActual(const char* moduleName, uintptr_t va) {
	ZHckModule* module = ZHckGetModule(moduleName);
	if (module == NULL) {
		return NULL;
	}

	return module->runtimeBase + (va - module->imageBase);
}

uintptr_t ZHckRelativeVirtualAddressToActual(const char* moduleName, uintptr_t rva) {
	ZHckModule* module = ZHckGetModule(moduleName);
	if (module == NULL) {
		return NULL;
	}

	return module->runtimeBase + rva;
}

