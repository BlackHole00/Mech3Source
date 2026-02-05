#pragma once

#include <string.h>

#define CMEM_ZERO(...) do { memset(&(__VA_ARGS__), 0, sizeof(__VA_ARGS__)); } while (0);
