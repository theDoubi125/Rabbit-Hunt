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
		m_cursor = 0;
	}

private:
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

template<typename T, int SIZE>
class queue
{
public:
	void enqueue(const T& data)
	{
		m_data[m_lastCursor] = data;
		m_lastCursor = (m_lastCursor+1)% SIZE;
	}

	void dequeueTo(T& outData)
	{
		outData = m_data[m_firstCursor];
		m_firstCursor = (m_firstCursor + 1) % SIZE;
	}

	T dequeue()
	{
		T result = m_data[m_firstCursor];
		m_firstCursor = (m_firstCursor + 1) % SIZE;
		return result;
	}

	int size() const
	{
		return (m_lastCursor - m_firstCursor + SIZE) % SIZE;
	}

	void getData(T* outData, int bufferSize) const
	{
		for (int i = 0; i < size() && i < bufferSize; i++)
		{
			outData[i] = m_data[(m_firstCursor + i) % SIZE];
		}
	}

private:
	T m_data[SIZE];
	int m_firstCursor = 0;
	int m_lastCursor = 0;
};