#pragma once

#include "character/character.h"
#include "level.h"

namespace editor
{
	namespace world
	{
		void drawWorldEditor(const character::manager& characterManager, const level::accessibilityMap& map);
	}
}