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
					ivec2 movement = getDirectionVector(currentStep.lookDirection) * currentStep.distance;
					paths[i].origin = paths[i].origin + movement;
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

				ivec2 movement = getDirectionVector(currentStep.lookDirection) * currentStep.distance;
				nextAction.data.direction = currentStep.lookDirection;
				nextAction.data.length = currentStep.distance; // optim : max = length as x = 0 or y = 0
				nextAction.data.duration = currentStep.distance;
				outActions->enqueue(nextAction);
			}
		}
		return true;
	}

	int compare(const ivec2& A, const ivec2& B)
	{
		int result = A.y - B.y;
		if (result == 0)
			result = A.x - B.x;
		return result;
	}

	void sortByXAndY(ivec2* tBase, ivec2* t, int count)
	{
		if (count >= 2)
		{
			//ImGui::Text("sort between %d and %d", (int)(t - tBase), (int)(t - tBase) + count);
			ivec2 pivot = t[count - 1];
			//ImGui::Text("pivot : (%d, %d)", pivot.x, pivot.y);
			int leftCursor = 0, rightCursor = count - 1;
			ivec2 buf;
			for (int i = 0; i < count - 1; i++)
			{
				if (compare(t[i], pivot) <= 0) // swap this element with the last one greater than the pivot
				{
					buf = t[i];
					t[i] = t[leftCursor];
					t[leftCursor] = buf;
					leftCursor++;
				}
			}
			t[count - 1] = t[leftCursor];
			t[leftCursor] = pivot;

			/*for (ivec2* i = tBase; i < t + count; i++)
			{
			if (i != tBase)
			ImGui::SameLine();
			if (i >= t)
			{
			if (*i == pivot)
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0, 0, 1));
			else
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0.5f, 0.5f, 1));
			ImGui::Text("(%d, %d)", i->x, i->y);
			ImGui::PopStyleColor();
			}
			else
			ImGui::Text("(%d, %d)", i->x, i->y);
			}*/
			sortByXAndY(tBase, t, leftCursor);
			if (leftCursor > 0)
				sortByXAndY(tBase, t + leftCursor, count - leftCursor);
			else sortByXAndY(tBase, t + 1, count - 1);
		}
	}
}