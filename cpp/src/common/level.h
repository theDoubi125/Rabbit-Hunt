#pragma once

#include "geometry.h"

#define MAX_LEVEL_SIZE 100


namespace level
{
	struct accessibilityMap
	{
		ivec2 min, size;
		unsigned int *data;

		bool isAccessible(const ivec2& cell) const;
		void setAccessible(const ivec2& cell, bool accessible);
		void setAllAccessible();
	};

	struct data
	{
		int tiles[MAX_LEVEL_SIZE * MAX_LEVEL_SIZE];
		ivec2 size;
	};
}