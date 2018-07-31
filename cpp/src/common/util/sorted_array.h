#pragma once

namespace array
{
	template<typename T>
	void insert(T* t, int index, const T& value, int currentArraySize)
	{
		T toAdd = value;
		T buffer;
		for (int i = index; i < currentArraySize; i++)
		{
			buffer = t[i];
			t[i] = toAdd;
			toAdd = buffer;
		}
		t[currentArraySize] = toAdd;
	}

	template<typename T>
	void addSorted(T* t, const T& value, int currentArraySize)
	{
		for (int i = 0; i < currentArraySize; i++)
		{
			if (t[i] > value)
			{
				insert(t, i, value, currentArraySize);
				return;
			}
		}
		t[currentArraySize] = value;
	}

	template<typename T>
	void addSortedWithIndex(T* t, int* indexArray, const T& value, int newIndex, int& arraySize)
	{
		for (int i = 0; i < arraySize; i++)
		{
			if (indexArray[i] > newIndex)
			{
				insert(indexArray, i, newIndex, arraySize);
				insert(t, i, value, arraySize);
				arraySize++;
				return;
			}
		}
		t[arraySize] = value;
		indexArray[arraySize] = newIndex;
		arraySize++;
	}

	template<typename T>
	inline int getIndexForSortInsert(const T* indexArray, const T& toAdd, int& arraySize)
	{
		for (int i = 0; i < arraySize; i++)
		{
			if (indexArray[i] > toAdd)
			{
				return i;
			}
		}
		return arraySize;
	}
}