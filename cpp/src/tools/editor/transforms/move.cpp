#include "move.h"
#include "path/path.h"
#include "memory/allocator.h"
#include "util/sorted_array.h"
#include "collision.h"

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

		void handleWallCollisions(move::MoveAssignmentContainer& assignment, character::manager& characters, const level::accessibilityMap& map)
		{
			vec2 velocities[MAX_CHARACTER_COUNT];
			parray<vec2, MAX_CHARACTER_COUNT> pos;
			const assignmentContainer& baseAssignment = assignment.assignmentContainerData;

			auto indexData = assignment.indexData;

			pushAllocatorStack();

			vec2* positions = mainAllocator->allocate<vec2>(indexData.characterCount);
			characters.get(indexData.sortedCharacters, positions, indexData.characterCount);
			struct TestData
			{
				character::handle character;
				ivec2 cellToTest;
			};
			
			buffer<ivec2> cells = mainAllocator->allocateBuffer<ivec2>(30);
			for (int i = 0; i < indexData.characterCount; i++)
			{
				character::handle character = indexData.sortedCharacters[i];
				vec2 velocity = indexData.globalVelocities[i];

				collision::Circle circle = { positions[i] + vec2(0.5f, 0.5f), 0.4f };

				action::direction directions[2];
				int dirCount = 0;

				path::getVectorDirections(velocity, directions, dirCount);

				for (int j = 0; j < dirCount; j++)
				{
					cells.clear();
					ivec2 dirVec = path::getDirectionVector(directions[j]);
					vec2 projectedVelocity = vec2(velocity.x * (float)abs(dirVec.x), velocity.y * (float)abs(dirVec.y));
					collision::getCellsTouchingCircleWithDirection(circle, cells, vec2(projectedVelocity.x, projectedVelocity.y));

					for (int k = 0; k < cells.size(); k++)
					{
						if (!map.isAccessible(cells.m_data[k])) // collision detection with map walls
						{
							actionData pushData = { (int)(1.0f / projectedVelocity.size()), 1,  path::getOppositeDirection(directions[j]) };
							assignment.addAction(character, pushData);

							vec2 replacementVec = vec2(-projectedVelocity.x, -projectedVelocity.y);
							characters.move(&character, &replacementVec, 1);
							break;
						}
					}
				}
			}
			popAllocatorStack();
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
				indexData.globalVelocities[i] = velocity;
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

		void MoveAssignmentContainer::addAction(const character::handle& character, const actionData& action)
		{
			auto& input = assignmentContainerData.input;
			auto& durations = assignmentContainerData.durations;
			auto& characters = assignmentContainerData.characters;
			auto& actionData = assignmentContainerData.actionData;
			auto& count = assignmentContainerData.count;
			int insertIndex = array::getIndexForSortInsert(durations, -action.duration, count);
			array::insert(durations, insertIndex, -action.duration, count);
			array::insert(characters, insertIndex, character, count);
			array::insert(actionData, insertIndex, action, count);

			addToIndex(character, action);

			count++;
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