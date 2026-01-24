#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <Windows.h>

#define ZHCK_MAX_MODULES 32
#define ZHCK_DEFAULT_MODULE NULL

typedef enum {
	ZHCK_VIRTUAL_ADDRESS = 0,
	ZHCK_RELATIVE_VIRTUAL_ADDRESS,
	ZHCK_ACTUAL_ADDRESS,
} ZHckAddressType;

typedef struct {
	const char* moduleName;
	uintptr_t targetCodeAddress;
	ZHckAddressType targetAddressType;
	void* replacementFunction;
} ZHckCodeReplacement;

typedef struct ZHckModule {
	const char* name;
	HMODULE module;
	uintptr_t runtimeBase;
	uintptr_t imageBase;
} ZHckModule;

typedef struct ZHckModuleDescriptor {
	const char* name;
	uintptr_t imageBase;
} ZHckModuleDescriptor;

typedef struct {
	// uintptr_t mech3RuntimeBase;
	ZHckModule modules[ZHCK_MAX_MODULES];
	size_t modulesCount;
} ZHack;
extern ZHack ZHck;

void ZHckInit(const ZHckModuleDescriptor* modules, size_t modulesCount);
void ZHckRegisterModule(const ZHckModuleDescriptor* descriptor);

ZHckModule* ZHckGetModule(const char* name);

void ZHckApplyCodeReplacement(const ZHckCodeReplacement* replacement);
void ZHckApplyCodeReplacements(const ZHckCodeReplacement* replacements, size_t replacementCount);

void ZHckReplaceBytes(void* target, const void* source, size_t length, bool didReplaceCode);

uintptr_t ZHckVirtualAddressToActual(const char* modeleName, uintptr_t va);
uintptr_t ZHckRelativeVirtualAddressToActual(const char* modeleName, uintptr_t rva);

template <class TMethodPtr>
void* ZHckMethodPtrToPtr(TMethodPtr methodPtr) {
	TMethodPtr methodPtrCpy = methodPtr;
	// NOTE: Highly dangerous undefined behaviour. Cpp does not like this...
	void* ptr = *(void**)&methodPtrCpy;
	return ptr;
}


