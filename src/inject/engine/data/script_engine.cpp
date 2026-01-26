#include "script_engine.h"

#include <string.h>
#include <Windows.h>

#include <engine/memory/memory.h>

ZGame ZGam;

ZDatScriptVariable* __thiscall ZDatScriptEngine::GetScriptVariablePtr(const char* variableName) {
	if (this->variablesListLength == 0) {
		return NULL;
	}

	for (size_t i = 0; i < this->variablesListLength; i++) {
		ZDatScriptVariable* variable = &this->variablesList[i];

		if (strcmp(variable->name, variableName) == 0) {
			return variable;
		}
	}

	return NULL;
}

bool __thiscall ZDatScriptEngine::SetVariableValue(char* variableName, uint32_t type, uint32_t value) {
	if (variableName == NULL || value == 0) {
		return false;
	}

	ZDatScriptVariable* variable = this->GetScriptVariablePtr(variableName);
	if (variable != NULL) {
		variable->value = value;
	} else {
		// NOTE: The variable is not in the list. Let's make room and add it.
		// 	This is really bad memory management, btw!
		this->variablesList = (ZDatScriptVariable*)ZMemRealloc(
			this->variablesList,
			(this->variablesListLength + 1) * sizeof(ZDatScriptVariable)
		);

		variable = &this->variablesList[this->variablesListLength];
		variable->name = ZMemStrDup(variableName);
		variable->type = type;
		variable->value = value;

		this->variablesListLength += 1;
	}

	return true;
}

void ZGamInit(void) {}


