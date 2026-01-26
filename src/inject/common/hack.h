#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <Windows.h>

#define CHCK_MAX_MODULES 32
#define CHCK_DEFAULT_MODULE NULL

typedef enum {
	CHCK_VIRTUAL_ADDRESS = 0,
	CHCK_RELATIVE_VIRTUAL_ADDRESS,
	CHCK_ACTUAL_ADDRESS,
} CHckAddressType;

typedef struct {
	const char* moduleName;
	uintptr_t targetCodeAddress;
	CHckAddressType targetAddressType;
	void* replacementFunction;
} CHckCodeReplacement;

typedef struct CHckModule {
	const char* name;
	HMODULE module;
	uintptr_t runtimeBase;
	uintptr_t imageBase;
} CHckModule;

typedef struct CHckModuleDescriptor {
	const char* name;
	uintptr_t imageBase;
} CHckModuleDescriptor;

typedef struct {
	CHckModule modules[CHCK_MAX_MODULES];
	size_t modulesCount;
} CHack;
extern CHack CHck;

void CHckInit(const CHckModuleDescriptor* modules, size_t modulesCount);
void CHckRegisterModule(const CHckModuleDescriptor* descriptor);

CHckModule* CHckGetModule(const char* name);

void CHckApplyCodeReplacement(const CHckCodeReplacement* replacement);
void CHckApplyCodeReplacements(const CHckCodeReplacement* replacements, size_t replacementCount);

void CHckReplaceBytes(void* target, const void* source, size_t length, bool didReplaceCode);

uintptr_t CHckVirtualAddressToActual(const char* modeleName, uintptr_t va);
uintptr_t CHckRelativeVirtualAddressToActual(const char* modeleName, uintptr_t rva);

template <class TMethodPtr>
void* CHckMethodPtrToPtr(TMethodPtr methodPtr) {
	TMethodPtr methodPtrCpy = methodPtr;
	// NOTE: Highly dangerous undefined behaviour. Cpp does not like this...
	void* ptr = *(void**)&methodPtrCpy;
	return ptr;
}


