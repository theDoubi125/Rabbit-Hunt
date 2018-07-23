#include "move.h"
#include "path/path.h"
#include "memory/allocator.h"
#include "util/sorted_array.h"

using namespace memory::util;

namespace action
{
	namespace transform
	{
		void move(const move::moveAssignmentContainer& assignment, character::manager& characters)
		{
			pushAllocatorStack();
			character::handle* handles = mainAllocator->allocate<character::handle>(assignment.m_count);
			vec2* velocities = mainAllocator->allocate<vec2>(assignment.m_count);
			getCurrentVelocities(assignment, velocities);
			characters.move(assignment.characters, velocities, assignment.m_count);
			popAllocatorStack();
		}

		void getCurrentVelocities(const move::moveAssignmentContainer& assignment, vec2* outVelocities)
		{
			memset(outVelocities, 0, MAX_CHARACTER_COUNT * sizeof(vec2));
			for (int i = 0; i < assignment.m_count; i++)
			{
				actionData data = assignment.actionData[i];
				vec2 dirVec = toFloatVec(path::getDirectionVector(data.direction));
				outVelocities[i] = dirVec * (float)data.length / (float)data.duration;
			}
		}

		void handleWallCollisions(const move::moveAssignmentContainer& assignment, character::manager& characters, const level::accessibilityMap& map)
		{
			vec2 velocities[MAX_CHARACTER_COUNT];
			parray<vec2, MAX_CHARACTER_COUNT> pos;
			
			for (int i = 0; i < assignment.m_count; i++)
			{
				actionData data = assignment.actionData[i];
				vec2 dirVec = toFloatVec(path::getDirectionVector(data.direction));
				velocities[i] = dirVec * ((float)data.length / (float)data.duration);
			}
			characters.move(assignment.characters, velocities, assignment.m_count);
		}
	}

	namespace move
	{
		void moveAssignmentContainer::add(character::handle character, int duration)
		{
			array::addSortedWithIndex(characters, durations, character, -duration, m_count);
		}

		void moveAssignmentContainer::handleInput()
		{
			const assignedAction *inputCharacters = input.getData();
			for (int i = 0; i < input.size(); i++)
			{
				int insertIndex = array::getIndexForSortInsert(durations, -inputCharacters[i].data.duration, m_count);
				array::insert(durations, insertIndex, -inputCharacters[i].data.duration, m_count);
				array::insert(characters, insertIndex, inputCharacters[i].character, m_count);
				array::insert(actionData, insertIndex, inputCharacters[i].data, m_count);
				m_count++;
			}
			input.clear();
		}

		void moveAssignmentContainer::update()
		{
			int firstToRemove = -1;
			for (int i = m_count - 1; i >= 0; i--)
			{
				durations[i]++;
				if (durations[i] >= 0)
				{
					firstToRemove = i;
				}
			}
			if (firstToRemove >= 0)
			{
				for (int i = 0; i < m_count - firstToRemove; i++)
					output.add(characters[i + firstToRemove]);
				m_count = firstToRemove;
			}
		}
	}
}