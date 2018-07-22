#pragma once

#include "geometry.h"
#include "memory/allocator.h"
#include "util/buffer.h"
#include "path.h"
#include "level.h"

namespace path
{
	namespace dijkstra
	{
		struct dijkstraMap
		{
			ivec2 min, size;
			ivec2 origin;
			ivec2* precedents;
			float* distances;

			inline int posToIndex(ivec2 pos) const
			{
				ivec2 relativePos = pos - min;
				return relativePos.x + size.x * relativePos.y;
			}
		};

		dijkstraMap dijkstra(ivec2 origin, int distance, const level::accessibilityMap& accessibility);
		void getPathTo(const dijkstraMap& data, ivec2 target, path& outPath);
	}
}