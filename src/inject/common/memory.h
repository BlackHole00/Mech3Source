#pragma once

#include <stdint.h>
#include <string.h>

#define CMEM_ZERO(...) memset(&(__VA_ARGS__), 0, sizeof(__VA_ARGS__))

#define CMEM_NULLREFERENCEINITIALIZER(...) (*reinterpret_cast<__VA_ARGS__*>(0))

#define CMEM_DUMP(...) CMemDump((void*)(__VA_ARGS__), sizeof(*(__VA_ARGS__)), #__VA_ARGS__, __FUNCTION__, __FILE__, __LINE__)
void CMemDump(void* ptr, size_t size, const char* expression, const char* function, const char* file, uint32_t line);
