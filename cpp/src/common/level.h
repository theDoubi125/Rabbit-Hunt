#pragma once

#include "geometry.h"

#define MAX_LEVEL_SIZE 100


namespace level
{
	struct data
	{
		int tiles[MAX_LEVEL_SIZE * MAX_LEVEL_SIZE];
		ivec2 size;
	};
}