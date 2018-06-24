#include "dijkstra.h"

namespace path
{
	namespace dijkstra
	{

		dijkstraMap dijkstra(ivec2 origin, int distance, memory::allocator& allocator)
		{
			dijkstraMap result;
			result.min = origin - ivec2(distance, distance);
			result.max = origin + ivec2(distance, distance);
			int columnCount = result.max.x - result.min.x + 1;
			int rowCount = result.max.y - result.min.y + 1;
			int cellCount = columnCount * rowCount;
			result.precedents = allocator.allocate<int>(cellCount);
			result.distances = allocator.allocate<float>(cellCount);
			for (int i = 0; i < cellCount; i++)
			{
				result.distances[i] = -1;
			}
			allocator.pushStack();

			struct posDist { ivec2 pos; float dist; };

			queue<posDist> toCompute = allocator.allocateQueue<posDist>(distance * distance);
			toCompute.enqueue({ origin, 0 });
			while (toCompute.size() > 0)
			{
				posDist data = toCompute.dequeue();
				ivec2 nextPos[] = { 
					data.pos + ivec2(1, 0), 
					data.pos + ivec2(0, 1),
					data.pos + ivec2(-1, 0), 
					data.pos + ivec2(0, -1),};
				for (int i = 0; i < 4; i++)
				{
					int cellIndex = result.posToIndex(nextPos[i]);
					if (result.distances[cellIndex] < 0 || result.distances[cellIndex] > data.dist + 1)
					{
						result.precedents[cellIndex] = result.posToIndex(;
					}
				}
			}
			allocator.popStack();
		}
	}
}