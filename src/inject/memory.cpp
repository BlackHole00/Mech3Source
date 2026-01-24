#include "memory.h"

#include "hack.h"

ZMemory ZMem;

void ZMemInit(void) {
	ZMem.malloc	= *(ZMemMallocProc* )ZHckVirtualAddressToActual(ZHCK_DEFAULT_MODULE, ZMEM_MECH3_MALLOC_VA );
	ZMem.calloc	= *(ZMemCallocProc* )ZHckVirtualAddressToActual(ZHCK_DEFAULT_MODULE, ZMEM_MECH3_CALLOC_VA );
	ZMem.realloc	= *(ZMemReallocProc*)ZHckVirtualAddressToActual(ZHCK_DEFAULT_MODULE, ZMEM_MECH3_REALLOC_VA);
	ZMem.free	= *(ZMemFreeProc*   )ZHckVirtualAddressToActual(ZHCK_DEFAULT_MODULE, ZMEM_MECH3_FREE_VA   );

	ZMem.strdup = *(ZMemStrDupProc*)ZHckVirtualAddressToActual(ZHCK_DEFAULT_MODULE, ZMEM_MECH3_STRDUP_VA);
}

void* ZMemAlloc(size_t size) {
	return ZMem.malloc(size);
}

void* ZMemAllocZ(size_t size) {
	return ZMem.calloc(1, size);
}

void* ZMemAllocE(size_t count, size_t size) {
	return ZMem.calloc(count, size);
}

void* ZMemRealloc(void* memory, size_t newSize) {
	return ZMem.realloc(memory, newSize);
}

void ZMemFree(void* memory) {
	ZMem.free(memory);
}

char* ZMemStrDup(const char* string) {
	return ZMem.strdup(string);
}


