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
			if (((A.min.x > B.min.x && B.max.x > A.min.x) || (A.min.x <= B.min.x && A.max.x > B.min.x))
				&& ((A.min.y > B.min.y && B.max.y > A.min.y) || (A.min.y <= B.min.y && A.max.y > B.min.y)))
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

	void testAABBCircleCollisions(const AABB* aabbs, const Circle* circles, BitArray* outFlags, int count)
	{
		const unsigned char BIT_ARRAY_SIZE = CHAR_BIT * sizeof(BitArray);
		int cellsCount = (count + BIT_ARRAY_SIZE - 1) / BIT_ARRAY_SIZE;
		for (int i = 0; i < cellsCount; i++)
		{																						//	  0 | 1 | 2
			outFlags[i] = 0;																	//	----|---|----
			for (int j = 0; j < BIT_ARRAY_SIZE &&j + i * BIT_ARRAY_SIZE < count; j++)			//	  3 | 4 | 5
			{																					//	----|---|----
				int index = j + i * BIT_ARRAY_SIZE;												//    6 | 7 | 8   
				const AABB& box = aabbs[index];							   
				const Circle& circle = circles[index];					                        
				int quadrant = 0;										                        
				if (circle.pos.x > box.max.x)
					quadrant += 2;
				else if(circle.pos.x > box.min.x)
					quadrant += 1;
				if (circle.pos.y > box.max.y)
					quadrant += 6;
				else if (circle.pos.y > box.min.y)
					quadrant += 3;

				vec2 corners[] = { box.min, {box.max.x, box.min.y}, vec2(), {box.min.x, box.max.y}, box.max };
				vec2 centerToCorner = corners[quadrant / 2] - circle.pos;

				switch (quadrant)
				{
				case 0:
				case 2:
				case 6:
				case 8:
					// center of circle in a diagonal => test distance to closer corner
					outFlags[i] |= ((centerToCorner.x * centerToCorner.x) + (centerToCorner.y * centerToCorner.y) < circle.r * circle.r) << j;
					break;
				case 1:
					// center of circle aligned along x or y => test distance to closer edge
					outFlags[i] |= (circle.pos.y + circle.r > box.min.y) << j;
					break;													
				case 3:														
					outFlags[i] |= (circle.pos.x + circle.r > box.min.x) << j;
					break;													
				case 5:														
					outFlags[i] |= (circle.pos.x - circle.r < box.max.x) << j;
					break;													
				case 7:														
					outFlags[i] |= (circle.pos.y - circle.r < box.max.y) << j;

					break;
				case 4:
					outFlags[i] |= 1 << j;
					break;
				}
			}
		}
	}
}