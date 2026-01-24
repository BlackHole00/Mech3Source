#pragma once

#define ZMEM_MECH3_MALLOC_VA 	0x005bb668
#define ZMEM_MECH3_CALLOC_VA 	0x005bb60c
#define ZMEM_MECH3_REALLOC_VA	0x005bb610
#define ZMEM_MECH3_FREE_VA 	0x005bb65c
#define ZMEM_MECH3_STRDUP_VA 	0x005bb670

typedef void* (__cdecl *ZMemMallocProc	)(size_t size);
typedef void* (__cdecl *ZMemCallocProc	)(size_t count, size_t size);
typedef void* (__cdecl *ZMemReallocProc	)(void* memory, size_t newSize);
typedef void  (__cdecl *ZMemFreeProc	)(void* memory);
typedef char* (__cdecl *ZMemStrDupProc	)(const char* string);

typedef struct ZMemory {
	ZMemMallocProc	malloc;
	ZMemCallocProc	calloc;
	ZMemReallocProc	realloc;
	ZMemFreeProc	free;
	ZMemStrDupProc	strdup;
} ZMemory;
extern ZMemory ZMem;

void ZMemInit(void);

void* ZMemAlloc(size_t size);
void* ZMemAllocZ(size_t size); // Alloc & zero-initialize
void* ZMemAllocE(size_t count, size_t size); // Alloc Elements (calls calloc)
void* ZMemRealloc(void* memory, size_t newSize);
void  ZMemFree(void* memory);

char* ZMemStrDup(const char* string);

