#pragma once

#include "geometry.h"
#include "memory/allocator.h"
#include "util/buffer.h"

namespace path
{
	namespace dijkstra
	{
		struct dijkstraMap
		{
			ivec2 min, size;
			ivec2 origin;
			int* precedents;
			float* distances;

			inline int posToIndex(ivec2 pos)
			{
				ivec2 relativePos = pos - min;
				return relativePos.x + size.x * relativePos.y;
			}
		};

		dijkstraMap dijkstra(ivec2 origin, int distance, memory::allocator& allocator);
	}
}