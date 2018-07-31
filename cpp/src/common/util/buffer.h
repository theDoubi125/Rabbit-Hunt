#pragma once

#include <iostream>

template<typename T>
class buffer
{
public:
	buffer(T* data, int* cursor, int size) : m_data(data), m_size(size), m_cursor(cursor) {}
	buffer() : m_data(nullptr), m_size(0), m_cursor(nullptr) {}

	void operator=(const buffer<T> other)
	{
		m_data = other.m_data;
		m_size = other.m_size;
		m_cursor = other.m_cursor;
	}

	inline void add(const T& data)
	{
		if (m_cursor != nullptr && *m_cursor < m_size)
		{
			m_data[*m_cursor] = data;
			(*m_cursor)++;
		}
		else std::cerr << "Error during add : full buffer" << std::endl;
	}

	inline const T* getData() const
	{
		return m_data;
	}

	int size() const
	{
		return *m_cursor;
	}

	void clear()
	{
		*m_cursor = 0;
	}

	T* m_data;
	int* m_cursor;
	int m_size;
};

template<typename T, int allocSize>
class allocatedBuffer
{
public:
	buffer<T> getRef()
	{
		buffer<T> result(m_data, &m_cursor, allocSize);
		return result;
	}

	void add(const T& data)
	{
		m_data[m_cursor] = data;
		m_cursor++;
	}

	const T* getData() const
	{
		return m_data;
	}

	int size() const
	{
		return m_cursor;
	}

	void clear()
	{
		m_cursor = 0;
	}

private:
	T m_data[allocSize];
	int m_cursor = 0;
};

template<typename T>
class queue
{
public:
	queue(T* data, int size) : m_data(data), m_size(size)
	{

	}

	queue() : m_data(nullptr), m_size(0)
	{

	}

	void enqueue(const T& data)
	{
		m_data[m_lastCursor] = data;
		m_lastCursor = (m_lastCursor+1) % m_size;
	}

	void dequeueTo(T& outData)
	{
		outData = m_data[m_firstCursor];
		m_firstCursor = (m_firstCursor + 1) % m_size;
	}

	T dequeue()
	{
		T result = m_data[m_firstCursor];
		m_firstCursor = (m_firstCursor + 1) % m_size;
		return result;
	}

	int size() const
	{
		return (m_lastCursor - m_firstCursor + m_size) % m_size;
	}

	void getData(T* outData, int bufferSize) const
	{
		for (int i = 0; i < size() && i < bufferSize; i++)
		{
			outData[i] = m_data[(m_firstCursor + i) % m_size];
		}
	}

	T* m_data;
	int m_firstCursor = 0;
	int m_lastCursor = 0;
	int m_size;

	class iterator
	{
	public:
		iterator(queue<T>& target) : m_target(target) {}
		T* value() const { return m_target.m_data[(m_target.m_firstCursor + m_pos) % m_target.m_size]; }
		void operator++() { m_pos++; }
		bool endReached() const { return ((m_target.m_firstCursor + m_pos) % m_target.m_size) == m_target.m_lastCursor; }

	private:
		int m_pos = 0;
		queue<T>& m_target;
	};

	class const_iterator
	{
	public:
		const_iterator(const queue<T>& target) : m_target(target) {}
		const T& value() const { return m_target.m_data[(m_target.m_firstCursor + m_pos) % m_target.m_size]; }
		const_iterator operator++(int x) { m_pos++; return *this; }
		bool endReached() const { return ((m_target.m_firstCursor + m_pos) % m_target.m_size) == m_target.m_lastCursor; }

	private:
		int m_pos = 0;
		const queue<T>& m_target;
	};

	iterator getIterator() { return iterator(*this); }
	const_iterator getIterator() const { return const_iterator(*this); }
};