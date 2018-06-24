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
			result.precedents = allocator.allocate<int>(distance * distance * 4);
			allocator.pushStack();
			queue<ivec2> toCompute = allocator.allocateQueue<ivec2>(distance * distance);
			toCompute.enqueue(origin);
			while (toCompute.size() > 0)
			{
				ivec2 pos = toCompute.dequeue();

			}
			allocator.popStack();
		}
	}
}