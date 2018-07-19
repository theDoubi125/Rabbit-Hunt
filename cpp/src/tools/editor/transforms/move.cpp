#include "move.h"

namespace action
{
	namespace transform
	{
		void move(const assignmentContainer& assignmentContainer, character::manager& characters)
		{
			vec2 velocities[MAX_CHARACTER_COUNT];
			vec2 directionVectors[4] = { vec2(0, -1), vec2(0, 1), vec2(-1, 0), vec2(1, 0) };
			for (int i = 0; i < assignmentContainer.m_count; i++)
			{
				velocities[i] = directionVectors[assignmentContainer.m_directions[i]];
			}
			characters.move(assignmentContainer.m_characters, velocities, assignmentContainer.m_count);
		}
	}
}