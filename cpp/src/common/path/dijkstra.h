#pragma once

#include "geometry.h"
#include "memory/allocator.h"
#include "util/buffer.h"
#include "path.h"

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

		struct accessibilityMap
		{
			ivec2 min, size;
			unsigned int *data;

			bool isAccessible(const ivec2& cell) const;
			void setAccessible(const ivec2& cell, bool accessible);
			void setAllAccessible();
		};

		dijkstraMap dijkstra(ivec2 origin, int distance, memory::allocator& allocator, const accessibilityMap& accessibility);
		void getPathTo(const dijkstraMap& data, ivec2 target, path& outPath, memory::allocator& allocator);
	}
}