#include "dijkstra.h"

namespace path
{
	namespace dijkstra
	{
		bool accessibilityMap::isAccessible(const ivec2& cell) const
		{
			if (cell.x < min.x || cell.y < min.y || cell.x >= (min + size).x || cell.y >= (min + size).y)
				return false;
			ivec2 relPos = cell - min;
			int cellIndex = relPos.x + relPos.y * size.x;
			int arrayIndex = cellIndex / (8 * sizeof(int));
			int arrayOffset = cellIndex % (8 * sizeof(int));
			return data[arrayIndex] & (1 << arrayOffset);
		}

		void accessibilityMap::setAccessible(const ivec2& cell, bool accessible)
		{
			if (cell.x < min.x || cell.y < min.y || cell.x >= (min + size).x || cell.y >= (min + size).y)
				return;
			ivec2 relPos = cell - min;
			int cellIndex = relPos.x + relPos.y * size.x;
			int arrayIndex = cellIndex / (8 * sizeof(int));
			int arrayOffset = cellIndex % (8 * sizeof(int));
			if(accessible)
				data[arrayIndex] |= (1 << arrayOffset);
			else data[arrayIndex] &= ~(1 << arrayOffset);
		}

		void accessibilityMap::setAllAccessible()
		{
			for (int i = 0; i < (size.x * size.y)/(8 * sizeof(int)) + 1; i++)
			{
				data[i] = -1;
			}
		}

		dijkstraMap dijkstra(ivec2 origin, int distance, memory::allocator& allocator, const accessibilityMap& accessibility)
		{
			dijkstraMap result;
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
						result.precedents[cellIndex] = nextPos[i];
						result.distances[cellIndex] = data.dist + 1;
						toCompute.enqueue({ nextPos[i], data.dist + 1 });
					}
				}
			}
			allocator.popStack();
			return result;
		}
	}
}