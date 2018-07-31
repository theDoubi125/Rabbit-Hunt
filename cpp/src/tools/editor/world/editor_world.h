#pragma once

#include "character/character.h"
#include "level.h"
#include "character/action/scheduler.h"
#include "collision.h"

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

		struct CollisionToolData
		{
			collision::Circle circle;
			vec2 movingDirection;
		};

		enum class editorTool
		{
			Window,
			Wall,
			Path,
			Collision,

			Last
		};

		extern char* editorToolNames[];

		struct editorData
		{
			editorTool selectedTool = editorTool::Window;
			int cellSize = 20;

			struct toolData { pathToolData path; wallToolData wall; CollisionToolData collision; };

			toolData selectedToolData = {};
		};

		void drawWorldEditor(editorData& data, const character::manager& characterManager, action::scheduler::manager& scheduler, level::accessibilityMap& map);
		ivec2 screenToWorld(const level::accessibilityMap& map, ivec2 pos, float cellSize);
		ivec2 worldToScreen(const level::accessibilityMap& map, ivec2 pos, float cellSize);
	}
}