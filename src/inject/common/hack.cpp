#include "hack.h"

#include <assert.h>
#include <cstdint>

CHack CHck;

void CHckInit(const CHckModuleDescriptor* modules, size_t modulesCount) {
	assert(modulesCount <= CHCK_MAX_MODULES && "Too many modules specified.");

	CHck.modulesCount = modulesCount;
	for (size_t i = 0; i < modulesCount; i++) {
		const CHckModuleDescriptor* descriptor = &modules[i];
		CHckRegisterModule(descriptor);
	}
}

void CHckRegisterModule(const CHckModuleDescriptor* descriptor) {
	assert(CHck.modulesCount < CHCK_MAX_MODULES && "Too many modules specified.");

	CHckModule* module = &CHck.modules[CHck.modulesCount];

	module->name = descriptor->name;
	module->module = GetModuleHandleA(descriptor->name);
	module->imageBase = (uintptr_t)descriptor->imageBase;
	module->runtimeBase = (uintptr_t)module->module;

	assert(module->module != NULL && "Trying to register an invalid module.");

	CHck.modulesCount++;
}

CHckModule* CHckGetModule(const char* name) {
	for (size_t i = 0; i < CHck.modulesCount; i++) {
		CHckModule* module = &CHck.modules[i];

		if ((name == NULL && module->name != NULL) || (name != NULL && module->name == NULL)) {
			continue;
		}

		if ((module->name == NULL && name == NULL) || strcmp(module->name, name) == 0) {
			return module;
		}
	}

	return NULL;
}

void CHckApplyCodeReplacement(const CHckCodeReplacement* replacement) {
	void* targetAddress;
	switch (replacement->targetAddressType) {
	case CHCK_VIRTUAL_ADDRESS: {
			targetAddress = (void*)CHckVirtualAddressToActual(replacement->moduleName, replacement->targetCodeAddress);
			break;
		}
	case CHCK_RELATIVE_VIRTUAL_ADDRESS: {
			targetAddress = (void*)CHckRelativeVirtualAddressToActual(replacement->moduleName, replacement->targetCodeAddress);
			break;
		}
	case CHCK_ACTUAL_ADDRESS: {
			targetAddress = (void*)replacement->targetCodeAddress;
			break;
		}
	}

	assert(targetAddress != NULL && "Invalid code replacement: trying to replace NULL. Is the specified module correct?");

	uint8_t patch[5];
	intptr_t rel = (uint8_t*)replacement->replacementFunction - ((uint8_t*)targetAddress + 5);
	patch[0] = 0xE9;
	*(int32_t*)(patch + 1) = (int32_t)rel;
	
	CHckReplaceBytes(targetAddress, patch, 5, true);
}

void CHckApplyCodeReplacements(const CHckCodeReplacement* replacements, size_t replacementCount) {
	for (size_t i = 0; i < replacementCount; i++) {
		CHckApplyCodeReplacement(&replacements[i]);
	}
}

void CHckReplaceBytes(void* target, const void* source, size_t length, bool didReplaceCode) {
	DWORD oldProtect;

	VirtualProtect(target, length, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy(target, source, length);
	VirtualProtect(target, length, oldProtect, &oldProtect);

	if (didReplaceCode) {
		FlushInstructionCache(GetCurrentProcess(), target, length);
	}
}

uintptr_t CHckVirtualAddressToActual(const char* moduleName, uintptr_t va) {
	CHckModule* module = CHckGetModule(moduleName);
	if (module == NULL) {
		return NULL;
	}

	return module->runtimeBase + (va - module->imageBase);
}

uintptr_t CHckRelativeVirtualAddressToActual(const char* moduleName, uintptr_t rva) {
	CHckModule* module = CHckGetModule(moduleName);
	if (module == NULL) {
		return NULL;
	}

	return module->runtimeBase + rva;
}

