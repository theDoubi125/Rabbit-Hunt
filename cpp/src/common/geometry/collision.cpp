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

				quadrant += 2 * (circle.pos.x > box.max.x);
				quadrant += 1 * (circle.pos.x <= box.max.x && circle.pos.x > box.min.x);
				quadrant += 6 * (circle.pos.y > box.max.y);
				quadrant += 3 * (circle.pos.y <= box.max.y && circle.pos.y > box.min.y);

				vec2 corners[] = { box.min, {box.max.x, box.min.y}, vec2(), {box.min.x, box.max.y}, box.max };
				vec2 centerToCorner = corners[quadrant / 2] - circle.pos;

				bool testByQuadrant[] =
				{
					(centerToCorner.x * centerToCorner.x) + (centerToCorner.y * centerToCorner.y) < circle.r * circle.r,
					circle.pos.y + circle.r > box.min.y,
					(centerToCorner.x * centerToCorner.x) + (centerToCorner.y * centerToCorner.y) < circle.r * circle.r,
					circle.pos.x + circle.r > box.min.x,
					true,
					circle.pos.x - circle.r < box.max.x,
					(centerToCorner.x * centerToCorner.x) + (centerToCorner.y * centerToCorner.y) < circle.r * circle.r,
					circle.pos.y - circle.r < box.max.y,
					(centerToCorner.x * centerToCorner.x) + (centerToCorner.y * centerToCorner.y) < circle.r * circle.r
				};

				
				outFlags[i] |= testByQuadrant[quadrant] << j;
			}
		}
	}

	vec2 getClosestPointOfCell(const vec2& A, const ivec2& cell) // not centered cells => cell (0, 0) has min of (0f, 0f) and max (1f, 1f)
	{
		int quadrant = 0;
		vec2 min = toFloatVec(cell);
		vec2 max = toFloatVec(cell + ivec2(1, 1));
		quadrant += 2 * (A.x > max.x);
		quadrant += 1 * (A.x > min.x && A.x <= max.x);
		quadrant += 6 * (A.y > max.y);
		quadrant += 3 * (A.y > min.y&& A.y <= max.y);

		vec2 closestPointByQuadrant[] =
		{
			vec2(0, 0),
			vec2(A.x - (float)cell.x, 0),
			vec2(1, 0),
			vec2(0, A.y - (float)cell.y),
			vec2(A.x - (float)cell.x, A.y - (float)cell.y),
			vec2(1, A.y - (float)cell.y),
			vec2(0, 1),
			vec2(A.x - (float)cell.x, 1),
			vec2(1, 1)
		};
		return min + closestPointByQuadrant[quadrant];
	}

	void getCellsTouchingCircle(const Circle& circle, ivec2 outCells, int cellCount)
	{
		for (int i = 0; i < (int)(circle.r + 1); i++)
		{
			
		}
	}
}