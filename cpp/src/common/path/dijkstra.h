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
			ivec2 min, max;
			ivec2 origin;
			int* precedents;
		};

		dijkstraMap dijkstra(ivec2 origin, int distance, memory::allocator& allocator);
	}
}