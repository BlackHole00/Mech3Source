#include "memory.h"

#include <ctype.h>

#include <common/trace.h>

void CMemDump(void* ptr, size_t size, const char* expression, const char* function, const char* file, uint32_t line) {

	CTrcTrace("[%s (%s:%d)] Memory dump of `%s` (0x%p - 0x%x bytes):\n", function, file, line, expression, ptr, size);

	for (size_t i = 0; i < (size / 4); i++) {
		void* currentWord = (void*)((uint32_t*)ptr + i);

		unsigned char chars[4];
		chars[0] = *((unsigned char*)currentWord + 0);
		chars[1] = *((unsigned char*)currentWord + 1);
		chars[2] = *((unsigned char*)currentWord + 2);
		chars[3] = *((unsigned char*)currentWord + 3);

		for (int j = 0; j < 4; j++) {
			if (iscntrl(chars[j])) {
				chars[j] = '?';
			}
		}

		CTrcTrace(
			"[%s (%s:%d)]\taddr=0x%08x  +0x%04x:\t    0x%08X        u=  %10u    i= %11d    '%c%c%c%c'    f= %f\n",
			function,
			file,
			line,
			currentWord,
			i * 4,
			*(uint32_t*)currentWord,
			*(uint32_t*)currentWord,
			*(int32_t*)currentWord,
			chars[0],
			chars[1],
			chars[2],
			chars[3],
			*(float*)currentWord
		);
	}
}
