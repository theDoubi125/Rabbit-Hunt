#pragma once
#include "util/buffer.h"
#include "character/action/common.h"

#define MAX_PATH_SIZE 100

namespace path
{
	struct step
	{
		action::direction direction;
		int distance;
	};

	struct path
	{
		ivec2 origin;
		queue<step, MAX_PATH_SIZE> steps;
	};
}