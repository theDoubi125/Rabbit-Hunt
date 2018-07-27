#include "collision.h"
#include <climits>
#include <cmath>

namespace collision
{
	void testAABBCollisions(const AABB* firsts, const AABB* seconds, BitArray* outFlags, int count)
	{
		static unsigned char bitArraySize = CHAR_BIT * sizeof(BitArray);
		for(int i=0; i<count; i++)
		{
			const AABB& A = firsts[i];
			const AABB& B = seconds[i];

			unsigned char flagNum = i%bitArraySize;
			unsigned char outFlagIndex = i / bitArraySize;
			if ((A.min.x > B.min.x && B.max.x > A.min.x) || (A.min.x <= B.min.x && A.max.x > B.min.x)
				&& (A.min.y > B.min.y && B.max.y > A.min.y) || (A.min.y <= B.min.y && A.max.y > B.min.y))
			{
				outFlags[outFlagIndex] |= 1 << flagNum;
			}
			else
			{
				outFlags[outFlagIndex] &= ~(1 << flagNum);
			}
		}
	}

	void testCircleCollisions(const Circle* firsts, const Circle* seconds, BitArray* outFlags, int count)
	{
		const unsigned char BIT_ARRAY_SIZE = CHAR_BIT * sizeof(BitArray);
		int cellsCount = (count + BIT_ARRAY_SIZE - 1) / BIT_ARRAY_SIZE;
		for (int i = 0; i < cellsCount; i++)
		{
			outFlags[i] = 0;
			for (int j = 0; j + i * BIT_ARRAY_SIZE < count; j++)
			{
				int index = j + i * BIT_ARRAY_SIZE;
				const Circle& A = firsts[index];
				const Circle& B = seconds[index];
				const vec2 AB = B.pos - A.pos;
				float distanceSquared = AB.x * AB.x + AB.y * AB.y;

				outFlags[i] |= (distanceSquared < (A.r + B.r) * (A.r + B.r)) << j;
			}
		}
	}

	void testAABBCircleCollisions(const Circle* circle, const AABB* aabb, int* outFlags, int count)
	{

	}
}