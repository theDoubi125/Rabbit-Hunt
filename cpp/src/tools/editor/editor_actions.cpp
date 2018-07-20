#include "editor_container.h"
#include "imgui/imgui.h"
#include "editor_main.h"
#include "util/packed_array.h"
#include "util/sorted_array.h"

#include <cstdio>
#include <vector>

#include "character/character.h"
#include "character/action/assignment_container.h"
#include "editor_util.h"

using namespace ImGui;

namespace editor
{
	void drawAssignmentEditor(char* label, action::assignmentContainer& idleManager)
	{
		static character::handle currentCharacter;
		static character::handle handledCharacters[MAX_CHARACTER_COUNT];
		static int duration;
		Begin(label);

		for (int i = 0; i < idleManager.m_count; i++)
		{
			Text("(id : %d, duration : %d, direction : %s)", idleManager.characters[i].id, -idleManager.durations[i], getDirectionName(idleManager.actionData[i].direction));
		}

		DragInt("character id", &currentCharacter.id, 1, 0, 100);
		DragInt("Duration", &duration, 1, 0, 100);
		if (Button("Add"))
		{
			idleManager.add(currentCharacter, duration);
		}
		if (Button("Update"))
		{
			idleManager.update();
		}

		End();
	}

	void drawMoveEditor()
	{
		/*char buffer[100];
		static action::move::manager moves;
		static character::manager characterManager;
		static character::handle selectedCharacter;
		static vec2 selectedPos;
		Begin("Idle Editor");
		SliderFloat2("Pos", (float*)&selectedPos, -100, 100);
		if (Button("Add Character"))
		{
			characterManager.add(selectedPos);
		}

		sprintf_s(buffer, 100, "%d", selectedCharacter.id);

		character::handle handles[MAX_CHARACTER_COUNT];
		int characterCount = characterManager.getHandles(handles, MAX_CHARACTER_COUNT);
		vec2 positions[MAX_CHARACTER_COUNT];
		characterManager.get(handles, positions, characterCount);

		if (BeginCombo("Character select", buffer))
		{
			for (int i = 0; i < characterCount; i++)
			{
				sprintf_s(buffer, 100, "%d", handles[i].id);
				if (Selectable(buffer, handles[i] == selectedCharacter))
				{
					selectedCharacter = handles[i];
				}
			}
			EndCombo();
		}

		static vec2 movementDir;
		static int movementDuration;
		SliderFloat2("Movement Dir", (float*)&movementDir, -10, 10);
		SliderInt("Movement Duration", &movementDuration, 1, 100);

		if (Button("Add movement"))
		{
			moves.add(selectedCharacter, movementDuration, movementDir);
		}

		character::handle output[100];
		if (Button("Update Movement"))
			moves.update(characterManager, output);

		for (int i = 0; i < characterCount; i++)
		{
			Text("%d : (%f; %f)", handles[i].id, positions[i].x, positions[i].y);
		}

		for (int i = 0; i < moves.m_count; i++)
		{
			Text("moving : %d -> (%f; %f), %d", moves.m_characters[i].id, moves.m_movement[i].x, moves.m_movement[i].y, -moves.m_durations[i]);
		}

		End();*/
	}
}