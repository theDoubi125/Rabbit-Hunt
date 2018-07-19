#pragma once

#include "character/character.h"
#include "level.h"
#include "character/action/scheduler.h"

namespace editor
{
	namespace world
	{
		struct pathToolData
		{
			character::handle selectedCharacter = { -1 };
		};

		struct wallToolData
		{
			bool selectedTile;
		};

		enum class editorTool
		{
			Window,
			Wall,
			Path,

			Last
		};

		extern char* editorToolNames[];

		struct editorData
		{
			editorTool selectedTool = editorTool::Window;
			int cellSize = 20;

			struct toolData { pathToolData path; wallToolData wall; };

			toolData selectedToolData = {};
		};

		void drawWorldEditor(editorData& data, const character::manager& characterManager, action::scheduler::manager& scheduler, level::accessibilityMap& map);
	}
}