#pragma once
#include "character/action/assignment_container.h"
#include "level.h"

namespace action
{
	namespace transform
	{
		void move(const assignmentContainer& assignmentContainer, character::manager& characters);
		void handleWallCollisions(const assignmentContainer& assignmentContainer, character::manager& characters, const level::accessibilityMap& map);
	}
}