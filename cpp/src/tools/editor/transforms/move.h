#pragma once
#include "character/action/assignment_container.h"
#include "level.h"

namespace action
{
	/* Contains all actions of that type assigned to any entity. Sorted by time of ending 
	+ contains an index giving for each character handle the list of actions bound to it */
	namespace move
	{
		struct MoveAssignmentContainer
		{
		public:
			MoveAssignmentContainer();
			void handleInput();
			void addAction(const character::handle& character, const actionData& action);
			void update();

			void addToIndex(const character::handle& character, const actionData& action);

			assignmentContainer assignmentContainerData;

			struct characterActionCount { character::handle handle; int actionCount; };

			struct
			{
				character::handle sortedCharacters[MAX_CHARACTER_COUNT];
				int actionCountByCharacter[MAX_CHARACTER_COUNT];
				vec2 globalVelocities[MAX_CHARACTER_COUNT];
				int characterCount = 0;

			} indexData;
		};
	}

	namespace transform
	{
		void move(const move::MoveAssignmentContainer& assignmentContainer, character::manager& characters);
		void getCurrentVelocities(const move::MoveAssignmentContainer& assignment, vec2* outVelocities);
		void handleWallCollisions(move::MoveAssignmentContainer& assignmentContainer, character::manager& characters, const level::accessibilityMap& map);
		void handleCharacterCollisions(move::MoveAssignmentContainer& assignmentContainer, character::manager& characters);
	}

	
}