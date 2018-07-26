#include "move.h"
#include "path/path.h"
#include "memory/allocator.h"
#include "util/sorted_array.h"

using namespace memory::util;

namespace action
{
	namespace transform
	{
		void move(const move::MoveAssignmentContainer& assignment, character::manager& characters)
		{
			pushAllocatorStack();
			const assignmentContainer& baseAssignment = assignment.assignmentContainerData;
			character::handle* handles = mainAllocator->allocate<character::handle>(baseAssignment.count);
			vec2* velocities = mainAllocator->allocate<vec2>(baseAssignment.count);
			getCurrentVelocities(assignment, velocities);
			characters.move(baseAssignment.characters, velocities, baseAssignment.count);
			popAllocatorStack();
		}

		void getCurrentVelocities(const move::MoveAssignmentContainer& assignment, vec2* outVelocities)
		{
			memset(outVelocities, 0, MAX_CHARACTER_COUNT * sizeof(vec2));
			const assignmentContainer& baseAssignment = assignment.assignmentContainerData;
			for (int i = 0; i < baseAssignment.count; i++)
			{
				actionData data = baseAssignment.actionData[i];
				vec2 dirVec = toFloatVec(path::getDirectionVector(data.direction));
				outVelocities[i] = dirVec * (float)data.length / (float)data.duration;
			}
		}

		void handleWallCollisions(const move::MoveAssignmentContainer& assignment, character::manager& characters, const level::accessibilityMap& map)
		{
			vec2 velocities[MAX_CHARACTER_COUNT];
			parray<vec2, MAX_CHARACTER_COUNT> pos;
			const assignmentContainer& baseAssignment = assignment.assignmentContainerData;
			
			for (int i = 0; i < baseAssignment.count; i++)
			{
				actionData data = baseAssignment.actionData[i];
				vec2 dirVec = toFloatVec(path::getDirectionVector(data.direction));
				velocities[i] = dirVec * ((float)data.length / (float)data.duration);
			}
			characters.move(baseAssignment.characters, velocities, baseAssignment.count);
		}
	}

	namespace move
	{
		MoveAssignmentContainer::MoveAssignmentContainer()
		{

		}

		void MoveAssignmentContainer::addToIndex(const character::handle& character, const actionData& action)
		{
			vec2 dirVec = toFloatVec(path::getDirectionVector(action.direction));
			vec2 velocity = dirVec * ((float)action.length / (float)action.duration);

			int actionIndexForInsert = 0;
			int i = 0;
			for (i = 0; i < indexData.characterCount && indexData.sortedCharacters[i] < character; i++)
			{
				actionIndexForInsert += indexData.actionCountByCharacter[i];
			}
			if (i >= indexData.characterCount)
			{
				indexData.characterCount++;
				indexData.sortedCharacters[i] = character;
				indexData.actionCountByCharacter[i] = 1;
			}
			else if (character < indexData.sortedCharacters[i]) // case where the character is not present yet
			{
				array::insert(indexData.sortedCharacters, i, character, indexData.characterCount);
				array::insert(indexData.actionCountByCharacter, i, 1, indexData.characterCount);

				array::insert(indexData.globalVelocities, actionIndexForInsert, velocity, assignmentContainerData.count);
			}
			else // case where there the character already has an action
			{
				indexData.actionCountByCharacter[i]++;;
				indexData.globalVelocities[i] += velocity;
			}
		}

		void MoveAssignmentContainer::handleInput()
		{
			auto& input =		assignmentContainerData.input;
			auto& durations =	assignmentContainerData.durations;
			auto& characters =	assignmentContainerData.characters;
			auto& actionData =	assignmentContainerData.actionData;
			auto& count =		assignmentContainerData.count;
			const assignedAction *inputCharacters = input.getData();
			for (int i = 0; i < input.size(); i++)
			{
				int insertIndex = array::getIndexForSortInsert(durations, -inputCharacters[i].data.duration, count);
				array::insert(durations, insertIndex, -inputCharacters[i].data.duration, count);
				array::insert(characters, insertIndex, inputCharacters[i].character, count);
				array::insert(actionData, insertIndex, inputCharacters[i].data, count);

				addToIndex(inputCharacters[i].character, inputCharacters[i].data);
				
				count++;

				
			}
			input.clear();
			// update the index
			
		}

		void MoveAssignmentContainer::update()
		{
			int firstToRemove = -1;
			for (int i = assignmentContainerData.count - 1; i >= 0; i--)
			{
				assignmentContainerData.durations[i]++;
				if (assignmentContainerData.durations[i] >= 0)
				{
					firstToRemove = i;
				}
			}
			if (firstToRemove >= 0)
			{
				for (int i = 0; i < assignmentContainerData.count - firstToRemove; i++)
				{
					character::handle character = assignmentContainerData.characters[i + firstToRemove];
					assignmentContainerData.output.add(character);
					// Find the character in the indexData to substract the action velocity
					int j = 0;
					for (j = 0; j < assignmentContainerData.count && indexData.sortedCharacters[i] != character; j++);
					if (j < assignmentContainerData.count)
					{
						const action::actionData& action = assignmentContainerData.actionData[i + firstToRemove];

						vec2 dirVec = toFloatVec(path::getDirectionVector(action.direction));
						vec2 velocity = dirVec * ((float)action.length / (float)action.duration);

						indexData.globalVelocities[j] -= velocity;
						indexData.actionCountByCharacter[j]--;
					}
				}
				assignmentContainerData.count = firstToRemove;
			}
		}
	}
}