#include "dijkstra.h"

namespace path
{
	namespace dijkstra
	{
		dijkstraMap dijkstra(ivec2 origin, int distance, memory::allocator& allocator, const level::accessibilityMap& accessibility)
		{
			dijkstraMap result;
			result.origin = origin;
			result.min = origin - ivec2(distance, distance);
			result.size = ivec2(distance * 2 + 1, distance * 2 + 1);
			int cellCount = result.size.x * result.size.y;
			result.precedents = allocator.allocate<ivec2>(cellCount);
			result.distances = allocator.allocate<float>(cellCount);
			for (int i = 0; i < cellCount; i++)
			{
				result.distances[i] = -1;
			}
			allocator.pushStack();

			struct posDist { ivec2 pos; float dist; };

			queue<posDist> toCompute = allocator.allocateQueue<posDist>(distance * distance);
			toCompute.enqueue({ origin, 0 });
			result.distances[result.posToIndex(origin)] = 0;
			while (toCompute.size() > 0)
			{
				posDist data = toCompute.dequeue();
				if (data.dist > distance)
					continue;
				ivec2 nextPos[] = { 
					data.pos + ivec2(1, 0), 
					data.pos + ivec2(0, 1),
					data.pos + ivec2(-1, 0), 
					data.pos + ivec2(0, -1),};
				for (int i = 0; i < 4; i++)
				{
					int cellIndex = result.posToIndex(nextPos[i]);
					bool bIsInBound = nextPos[i].x >= result.min.x && nextPos[i].y >= result.min.y && nextPos[i].x < result.min.x + result.size.x && nextPos[i].y < result.min.y + result.size.y;
					if (bIsInBound && accessibility.isAccessible(nextPos[i]) && result.distances[cellIndex] < 0 || result.distances[cellIndex] > data.dist + 1)
					{
						result.precedents[cellIndex] = data.pos;
						result.distances[cellIndex] = data.dist + 1;
						toCompute.enqueue({ nextPos[i], data.dist + 1 });
					}
				}
			}
			allocator.popStack();
			return result;
		}

		void getPathTo(const dijkstraMap& data, ivec2 target, path& outPath, memory::allocator& allocator)
		{
			allocator.pushStack();
			buffer<ivec2> reversedPath = allocator.allocateBuffer<ivec2>(outPath.steps.m_size);
			reversedPath.add(target);
			if (data.distances[data.posToIndex(target)] >= 0)
			{
				ivec2 cursorPos = target;
				while (cursorPos != data.origin)
				{
					cursorPos = data.precedents[data.posToIndex(cursorPos)];
					reversedPath.add(cursorPos);
				}
			}

			ivec2 cursorPos = data.origin;
			for (int i = reversedPath.size() - 1; i >= 0; i--)
			{
				outPath.steps.enqueue({ reversedPath.m_data[i] - cursorPos, action::direction::RIGHT });
			}
			allocator.popStack();
		}
	}
}