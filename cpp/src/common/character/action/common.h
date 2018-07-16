#pragma once

#include <string>
#include "util/buffer.h"
#include "character/character.h"

namespace action
{
	enum type
	{
		NONE,
		IDLE,
		MOVING,

		LAST,
	};

	enum direction
	{
		UP,
		DOWN,
		LEFT,
		RIGHT,
		UNDEFINED
	};

	extern std::string typeNames[50];

	struct actionData
	{
		int duration;
		int length;
		direction direction;
	};

	struct typedActionData
	{
		type action;
		actionData data;
	};

	struct assignedAction
	{
		character::handle character;
		actionData data;
	};

	using characterBuffer = allocatedBuffer<character::handle, MAX_CHARACTER_COUNT>;
}