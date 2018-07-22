#include "move.h"
#include "path/path.h"

namespace action
{
	namespace transform
	{
		void move(const assignmentContainer& assignmentContainer, character::manager& characters)
		{
			vec2 velocities[MAX_CHARACTER_COUNT];
			for (int i = 0; i < assignmentContainer.m_count; i++)
			{
				actionData data = assignmentContainer.actionData[i];
				vec2 dirVec = toFloatVec(path::getDirectionVector(data.direction));
				velocities[i] = dirVec * (float)data.length / (float)data.duration;
			}
			characters.move(assignmentContainer.characters, velocities, assignmentContainer.m_count);
		}

		void handleWallCollisions(const assignmentContainer& assignmentContainer, character::manager& characters, const level::accessibilityMap& map)
		{
			vec2 velocities[MAX_CHARACTER_COUNT];
			parray<vec2, MAX_CHARACTER_COUNT> pos;
			
			for (int i = 0; i < assignmentContainer.m_count; i++)
			{
				actionData data = assignmentContainer.actionData[i];
				vec2 dirVec = toFloatVec(path::getDirectionVector(data.direction));
				velocities[i] = dirVec * data.length / data.duration;
			}
			characters.move(assignmentContainer.characters, velocities, assignmentContainer.m_count);
		}
	}
}