#pragma once

#include "character/action/common.h"

namespace ImGui
{
	char* getDirectionName(const action::direction& direction);
	bool DirectionSelector(char* label, action::direction* direction);
}