#pragma once

#include <corecrt.h>
#include <stddef.h>
#include <stdint.h>

#define ZGAM_SCRIPTENGINE_VA "TO FIND"

typedef struct ZDatScriptVariable {
	const char* name;
	uint32_t type;
	uint32_t value;
} ZDatScriptVariable;

typedef class ZDatScriptEngine {
public:
	// fileFormat is ".zbd"
	ZDatScriptEngine(const char* fileName, const char* fileFormat);

	virtual void virtual1();
	virtual void virtual2();
	virtual void virtual3();

	ZDatScriptVariable* __thiscall GetScriptVariablePtr(const char* variableName);
	bool __thiscall SetVariableValue(char* variableName, uint32_t type, uint32_t value);

	const uint8_t unks[0x68 - sizeof(void*)];
	ZDatScriptVariable* variablesList;
	uint32_t variablesListLength;
	const uint8_t byte[0xBC - 0x68 - 4 - sizeof(void*)];
} ZDatScriptEngine;
_STATIC_ASSERT(sizeof(ZDatScriptEngine) == 0xBC);
_STATIC_ASSERT(offsetof(ZDatScriptEngine, variablesList) == 0x68);
_STATIC_ASSERT(offsetof(ZDatScriptEngine, variablesListLength) == 0x6C);

typedef class ZGamScriptEngine: public ZDatScriptEngine {
public:
	ZGamScriptEngine();
} ZGamScriptEngine;
_STATIC_ASSERT(sizeof(ZGamScriptEngine) == 0xBC);

typedef struct ZGam {
	ZGamScriptEngine* scriptEngine;
} ZGame;
extern ZGame ZGam;

void ZGamInit(void);

