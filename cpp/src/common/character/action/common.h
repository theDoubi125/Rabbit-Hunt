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
		type action;
		int duration;
		direction direction;
	};

	struct assignedAction
	{
		character::handle character;
		int duration;
		direction direction;
	};

	using characterBuffer = allocatedBuffer<character::handle, MAX_CHARACTER_COUNT>;
}