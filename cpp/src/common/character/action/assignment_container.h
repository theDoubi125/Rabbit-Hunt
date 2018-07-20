#pragma once
#include "character/character.h"
#include "common.h"

#include "util/buffer.h"

namespace action
{
	class assignmentContainer;
}

namespace editor
{
	void drawAssignmentEditor(char* label, action::assignmentContainer& idleManager);
}

namespace action
{
	class assignmentContainer
	{
	public:
		void add(character::handle handle, int duration);
		void handleInput();
		void update();

		buffer<character::handle> output;
		allocatedBuffer<assignedAction, MAX_CHARACTER_COUNT> input;

		int durations[MAX_CHARACTER_COUNT];
		character::handle characters[MAX_CHARACTER_COUNT];
		action::actionData actionData[MAX_CHARACTER_COUNT];
		int m_count;

		friend void editor::drawAssignmentEditor(char* label, assignmentContainer& assignments);
	};
}