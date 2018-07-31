#pragma once

#include "vec.h"
#include "util/buffer.h"

namespace collision
{
	using BitArray = unsigned int;

	struct AABB
	{
		vec2 min, max;
	};

	struct Circle
	{
		vec2 pos;
		float r;
	};

	void testAABBCollisions(const AABB* firsts, const AABB* seconds, BitArray* outFlags, int count);
	void testCircleCollisions(const Circle* firsts, const Circle* seconds, BitArray* outFlags, int count);
	void testAABBCircleCollisions(const AABB* aabbs, const Circle* circles, BitArray* outFlags, int count);
	void getCellsTouchingCircle(const Circle& circle, buffer<ivec2>& outCells);
	void getCellsTouchingCircleWithDirection(const Circle& circle, buffer<ivec2>& outCells, const vec2& direction);
}