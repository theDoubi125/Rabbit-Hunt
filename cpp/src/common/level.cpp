#include "level.h"

namespace level
{
	bool accessibilityMap::isAccessible(const ivec2& cell) const
	{
		if (cell.x < min.x || cell.y < min.y || cell.x >= (min + size).x || cell.y >= (min + size).y)
			return false;
		ivec2 relPos = cell - min;
		int cellIndex = relPos.x + relPos.y * size.x;
		int arrayIndex = cellIndex / (8 * sizeof(int));
		int arrayOffset = cellIndex % (8 * sizeof(int));
		return (data[arrayIndex] & (1 << arrayOffset)) != 0;
	}

	void accessibilityMap::setAccessible(const ivec2& cell, bool accessible)
	{
		if (cell.x < min.x || cell.y < min.y || cell.x >= (min + size).x || cell.y >= (min + size).y)
			return;
		ivec2 relPos = cell - min;
		int cellIndex = relPos.x + relPos.y * size.x;
		int arrayIndex = cellIndex / (8 * sizeof(int));
		int arrayOffset = cellIndex % (8 * sizeof(int));
		if (accessible)
			data[arrayIndex] |= (1 << arrayOffset);
		else data[arrayIndex] &= ~(1 << arrayOffset);
	}

	void accessibilityMap::setAllAccessible()
	{
		for (size_t i = 0; i < (size.x * size.y) / (8 * sizeof(int)) + 1; i++)
		{
			data[i] = -1;
		}
	}
}