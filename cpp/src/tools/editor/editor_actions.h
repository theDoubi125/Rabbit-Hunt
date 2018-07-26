#pragma once

#include "character/character.h"
#include "editor/editor_util.h"

namespace action
{
	class assignmentContainer;
}

namespace editor
{
	template<typename T>
	void drawAssignmentEditor(char* label, T& idleManager)
	{
		static character::handle currentCharacter;
		static character::handle handledCharacters[MAX_CHARACTER_COUNT];
		static int duration;
		ImGui::Begin(label);

		for (int i = 0; i < idleManager.count; i++)
		{
			ImGui::Text("(id : %d, duration : %d, direction : %s)", idleManager.characters[i].id, -idleManager.durations[i], ImGui::getDirectionName(idleManager.actionData[i].direction));
		}

		ImGui::End();
	}

	void drawMoveEditor();

}