#pragma once

namespace editor
{
	namespace container
	{
		void drawPackedArrayEditor();
	}
}

template<typename T, int maxSize>
class parray
{
public:
	struct handle
	{
	public:
		bool operator<(const handle& other) const { return id < other.id; }
		bool operator==(const handle& other) const { return id == other.id; }
		bool operator!=(const handle& other) const { return id != other.id; }
		int id;
	};

	int getHandles(handle* out, int outSize) const
	{
		for (int i = 0; i < m_allocCursor; i++)
		{
			out[i].id = m_allocs[i];
		}
		return m_allocCursor;
	}

	handle add()
	{
		handle result;
		if (m_maxAllocCount <= m_allocCursor)
		{
			m_indirs[m_allocCursor] = m_allocCursor;
			m_allocs[m_allocCursor] = m_allocCursor;
			m_maxAllocCount++;
			result.id = m_allocCursor;
		}
		result.id = m_allocs[m_allocCursor];
		return result;
	}

	handle add(const T& data)
	{
		handle result;
		if (m_maxAllocCount <= m_allocCursor)
		{
			m_indirs[m_allocCursor] = m_allocCursor;
			m_allocs[m_allocCursor] = m_allocCursor;
			m_maxAllocCount++;
			result.id = m_allocCursor;
		}
		result.id = m_allocs[m_allocCursor];
		m_data[m_allocCursor] = data;
		m_allocCursor++;
		return result;
	}

	void remove(const handle& transform)
	{
		if (m_allocCursor > 0)
		{
			m_allocCursor--;
			int dataToSwap = m_indirs[transform.id];
			int nextAvailableCell = m_allocCursor;
			m_data[dataToSwap] = m_data[nextAvailableCell];

			int buffer = m_indirs[m_allocs[nextAvailableCell]];
			m_indirs[m_allocs[nextAvailableCell]] = m_indirs[transform.id];
			m_indirs[transform.id] = buffer;

			buffer = m_allocs[dataToSwap];
			m_allocs[dataToSwap] = m_allocs[nextAvailableCell];
			m_allocs[nextAvailableCell] = buffer;

		}
	}

	void set(handle* handles, T* data, int count)
	{
		for (int i = 0; i < count; i++)
		{
			handle& handle = handles[i];
			m_data[m_indirs[handle.id]] = data[i];
		}
	}

	void get(const handle* handles, T* outData, int count) const
	{
		for (int i = 0; i < count; i++)
		{
			outData[i] = m_data[m_indirs[handles[i].id]];
		}
	}

	int count() const
	{
		return m_allocCursor;
	}

	friend void editor::container::drawPackedArrayEditor();

protected:
	T m_data[maxSize];
	int	m_indirs[maxSize];
	int	m_allocs[maxSize];
	int m_allocCursor = 0, m_maxAllocCount = 0;
};