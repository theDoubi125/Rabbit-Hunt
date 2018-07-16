#include "path.h"

namespace path
{
	path* allocatePaths(memory::allocator& allocator, int pathsCount, int maxStepsCount)
	{
		path* result = allocator.allocate<path>(pathsCount);
		initializePaths(allocator, result, pathsCount, maxStepsCount);
		return result;
	}

	void initializePaths(memory::allocator& allocator, path* outPaths, int pathsCount, int maxStepsCount)
	{
		queue<step>* steps = allocator.allocate<queue<step>>(pathsCount);
		allocator.allocateQueues(steps, maxStepsCount, pathsCount);
		for (int i = 0; i < pathsCount; i++)
		{
			outPaths[i].steps.m_data = steps[i].m_data;
			outPaths[i].steps.m_size = steps[i].m_size;
		}
	}

	void advanceThroughPaths(path* paths, int pathCount, int steps)
	{
		for (int i = 0; i < steps; i++)
		{
			for (int j = 0; j < pathCount; j++)
			{
				if (paths[i].steps.size() > 0)
				{
					step currentStep = paths[i].steps.dequeue();
					paths[i].origin = paths[i].origin + currentStep.movement;
				}
			}
		}
	}

	bool toActionLists(const queue<step>* inPaths, queue<action::typedActionData>* outActions, int pathsCount, float velocity)
	{
		for (int i = 0; i < pathsCount; i++)
		{
			for (queue<step>::const_iterator it = inPaths[i].getIterator(); !it.endReached(); it++)
			{
				step currentStep = it.value();
				action::typedActionData nextAction;
				nextAction.action = action::type::MOVING;
				
				nextAction.data.direction = getVectorDirection(currentStep.movement);
				nextAction.data.duration = (int)(currentStep.movement.max()); // optim : max = length as x = 0 or y = 0
				outActions->enqueue(nextAction);
			}
		}
		return true;
	}
}