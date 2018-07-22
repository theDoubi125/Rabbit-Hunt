#pragma once

#include "editor/world/editor_world.h"
#include "character/action/scheduler.h"

namespace editor
{
	namespace tools
	{
		namespace pathTool
		{
			void updatePathTool(world::editorData& data, const character::manager& characterManager, action::scheduler::manager& scheduler, level::accessibilityMap& map);
		}
	}
}