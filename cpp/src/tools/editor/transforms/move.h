#pragma once
#include "character/action/assignment_container.h"
#include "level.h"

namespace action
{
	/* Contains all actions of that type assigned to any entity. Sorted by time of ending*/
	namespace move
	{
		class moveAssignmentContainer
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
		};
	}

	namespace transform
	{
		void move(const move::moveAssignmentContainer& assignmentContainer, character::manager& characters);
		void getCurrentVelocities(const move::moveAssignmentContainer& assignment, vec2* outVelocities);
		void handleWallCollisions(const move::moveAssignmentContainer& assignmentContainer, character::manager& characters, const level::accessibilityMap& map);
	}

	
}