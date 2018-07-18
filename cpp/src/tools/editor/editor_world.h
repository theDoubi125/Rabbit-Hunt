#pragma once

#include "character/character.h"
#include "level.h"

namespace editor
{
	namespace world
	{
		struct pathToolData
		{
			character::handle selectedCharacter = { -1 };
		};

		enum class editorTool
		{
			Wall,
			Path
		};

		struct editorData
		{
			editorTool selectedTool = editorTool::Wall;
			int cellSize = 20;

			union toolData { pathToolData path; };

			toolData selectedToolData = {};
		};

		void drawWorldEditor(editorData& data, const character::manager& characterManager, level::accessibilityMap& map);
	}
}