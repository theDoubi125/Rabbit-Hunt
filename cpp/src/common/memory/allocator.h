#pragma once

#include "util/buffer.h"

namespace memory
{
	class allocator
	{
	public:
		allocator(size_t size)
		{
			m_data = (char*)malloc(size);
			m_cursor = 0;
		}

		~allocator()
		{
			delete m_data;
		}

		template<typename T> 
		T* allocate(int count)
		{
			T* result = (T*)(m_data + m_cursor);
			m_cursor += sizeof(T) * count;
		}

		template<typename T>
		buffer<T> allocateBuffer(int size)
		{
			int* cursor = (int*)(m_data + m_cursor);
			m_cursor += sizeof(int);
			T* data = (T*)(m_data + m_cursor);
			buffer<T> result(data, cursor, size);
			m_cursor += sizeof(T) * size;
			return result;
		}

	private:
		char* m_data;
		size_t m_cursor;
		size_t m_size;
	};
}