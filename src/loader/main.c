#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <windows.h>

typedef struct Arguments {
	char* targetExe;
	char* targetCwd;
	char* injectionDll;
} Arguments;

char* GetDirectoryOfFile(const char* file) {
	int lastPathSeparatorIdx = -1;
	int i = 0;
	char currentChar;
        while ((currentChar = file[i]) != '\0') {
        	if (currentChar == '\\' || currentChar == '/') {
        		lastPathSeparatorIdx = i;
        	}

        	i++;
        }

        if (lastPathSeparatorIdx == -1) {
        	return NULL;
        } else {
	        char* directory = malloc(lastPathSeparatorIdx + 1);
	        memcpy(directory, file, lastPathSeparatorIdx);
	        directory[lastPathSeparatorIdx] = '\0';

	        return directory;
        }
}

void PrintUsage() {
	printf("USAGE: loader.exe [target_exe] [inject_dll]\n");
}

// NOTE: Will leak
bool ParseArguments(Arguments* arguments, int argc, char** argv) {
	argc--;
	argv++;

	if (argc > 2 || (argc >= 1 && (
		strcmp(argv[0], "help") == 0 || strcmp(argv[0], "-h") == 0 ||
		strcmp(argv[0], "-help") == 0 || strcmp(argv[0], "--help") == 0 ||
		strcmp(argv[0], "/?") == 0
	))) {
		PrintUsage();
		return false;
	}

	char loaderFileName[MAX_PATH] = { 0 };
	GetModuleFileNameA(NULL, loaderFileName, MAX_PATH);
	printf("%s\n", loaderFileName);

	char* loaderDirectory = GetDirectoryOfFile(loaderFileName);
	int loaderDirectoryLength = strlen(loaderDirectory);
	printf("%s\n", loaderDirectory);

	// NOTE: This switch will leak
	switch (argc) {
		case 0: {
			arguments->targetExe = "C:\\MicroProse\\MechWarrior3\\Mech3.exe";
			arguments->targetCwd = "C:\\MicroProse\\MechWarrior3";

			int suffixLen = strlen("\\inject.dll");
			arguments->injectionDll = malloc(loaderDirectoryLength + suffixLen);
			strcpy(arguments->injectionDll, loaderDirectory);
			strcat(arguments->injectionDll, "\\inject.dll");

			break;
		}
		case 1: {
			arguments->targetExe = argv[0];
			arguments->targetCwd = GetDirectoryOfFile(arguments->targetExe);

			int suffixLen = strlen("\\inject.dll");
			arguments->injectionDll = malloc(loaderDirectoryLength + suffixLen);
			strcpy(arguments->injectionDll, loaderDirectory);
			strcat(arguments->injectionDll, "\\inject.dll");
			break;
		}
		case 2: {
			arguments->targetExe = argv[0];
			arguments->targetCwd = GetDirectoryOfFile(arguments->targetExe);
			arguments->injectionDll = argv[1];
			break;
		}
	}

	free(loaderDirectory);
	return true;
}

int main(int argc, char** argv) {
	Arguments arguments;
	if (!ParseArguments(&arguments, argc, argv)) {
		return 1;
	}

	printf("Patching parameters:\n");
	printf("\t- Target executable: %s\n", arguments.targetExe);
	printf("\t- Target working directory: %s\n", arguments.targetCwd);
	printf("\t- Injection dll: %s\n", arguments.injectionDll);

	STARTUPINFOA startupInfo = { .cb = sizeof(startupInfo) };
	PROCESS_INFORMATION processInfo = { 0 };

	printf("Launching mech3 process...\n");
	if (!CreateProcessA(arguments.targetExe, NULL, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, arguments.targetCwd, &startupInfo, &processInfo)) {
		printf("Could not create the Mech3.exe process.\n");
		return -1;
	}

	printf("Allocating injection data...\n");
	void* dllNamePayload = VirtualAllocEx(processInfo.hProcess, NULL, 4096, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	WriteProcessMemory(processInfo.hProcess, dllNamePayload, arguments.injectionDll, strlen(arguments.injectionDll) + 1, NULL);

	LPTHREAD_START_ROUTINE loadLibraryProc = (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");

	printf("Injecting...\n");
	HANDLE hRemoteThread = CreateRemoteThread(processInfo.hProcess, NULL, 0, loadLibraryProc, dllNamePayload, 0, NULL);

	printf("Actually launching mech3...\n");
	WaitForSingleObject(hRemoteThread, INFINITE);
	VirtualFreeEx(processInfo.hProcess, dllNamePayload, 0, MEM_RELEASE);
	CloseHandle(hRemoteThread);

	ResumeThread(processInfo.hThread);

	CloseHandle(processInfo.hThread);
	CloseHandle(processInfo.hProcess);

	return 0;
}
