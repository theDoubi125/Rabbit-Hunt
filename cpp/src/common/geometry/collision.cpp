#include "collision.h"
#include <climits>

namespace collision
{
	void testAABBCollisions(const AABB* firsts, const AABB* seconds, BitArray* outFlags, int count)
	{
		for(int i=0; i<count; i++)
		{
			const AABB& A = firsts[i];
			const AABB& B = seconds[i];

			unsigned char bitArraySize = CHAR_BIT * sizeof(BitArray);
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

	void testCircleCollisions(const Circle* firsts, const Circle* seconds, int* outFlags, int count)
	{

	}

	void testAABBCircleCollisions(const Circle* circle, const AABB* aabb, int* outFlags, int count)
	{

	}
}