#pragma once

template<typename T, int maxSize>
class allocArray
{
public:
	int add(const T& data)
	{
		if (m_nextAllocsSize > 0)
		{
			int cell = m_nextAllocs[m_nextAllocsCursor];
			m_data[cell] = data;
			m_nextAllocsCursor = (m_nextAllocsCursor + 1) % maxSize;
			m_nextAllocsSize--;
			m_count++;
			return cell;
		}
		else
		{
			m_data[m_count] = data;
			m_count++;
			return m_count;
		}
	}

	void remove(int index)
	{
		int toInsert = index;
		for (int i = 0; i < m_nextAllocsSize; i++)
		{
			int index = (m_nextAllocsCursor + i) % maxSize;
			if (m_nextAllocs[index] > toInsert)
			{
				int buf = toInsert;
				toInsert = m_nextAllocs[index];
				m_nextAllocs[index] = toInsert;
			}
		}
		m_nextAllocsSize++;
	}

	T& get(int index)
	{
		return m_data[index];
	}

	T get(int index) const
	{
		return m_data[index];
	}

protected:
	T m_data[maxSize];
	// available cells sorted (only the m_nextAllocsSize first, from the cell m_nextAllocsCursor)
	int m_nextAllocs[maxSize];
	int m_count = 0;
	int m_nextAllocsCursor = 0, m_nextAllocsSize;
};