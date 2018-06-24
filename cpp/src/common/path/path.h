#pragma once
#include "util/buffer.h"
#include "character/action/common.h"
#include "memory/allocator.h"

#define MAX_PATH_SIZE 100

namespace path
{
	struct step
	{
		int distance;
		action::direction direction;
	};

	struct path
	{
		ivec2 origin;
		queue<step> steps;
	};

	inline ivec2 getDirectionVector(action::direction dir)
	{
		switch (dir)
		{
		case action::direction::UP:
			return ivec2(0, -1);
		case action::direction::DOWN:
			return ivec2(0, 1);
		case action::direction::LEFT:
			return ivec2(-1, 0);
		case action::direction::RIGHT:
			return ivec2(1, 0);
		}
	}

	path* allocatePaths(memory::allocator& allocator, int pathsCount, int maxStepsCount);
	void initializePaths(memory::allocator& allocator, path* outPaths, int pathsCount, int maxStepsCount);
	void advanceThroughPaths(path* paths, int pathCount, int steps);
	bool toActionLists(const queue<step>* inPaths, queue<action::actionData>* outActions, int pathsCount, float velocity);
	

}