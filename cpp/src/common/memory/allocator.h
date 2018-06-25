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
			m_stack = (size_t*)(m_data + size - sizeof(size_t));
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
			return result;
		}

		bool pushStack()
		{
			*m_stack = m_cursor;
			m_stack -= 1;
			return (void*)m_stack > (void*)(m_data + m_cursor);
		}
		
		bool popStack()
		{
			m_stack += 1;
			m_cursor = *m_stack;
			return (void*)m_stack < (void*)(m_data + m_size - sizeof(size_t));
		}

		template<typename T>
		buffer<T> allocateBuffer(int size)
		{
			int* cursor = (int*)(m_data + m_cursor);
			*cursor = 0;
			m_cursor += sizeof(int);
			T* data = (T*)(m_data + m_cursor);
			buffer<T> result(data, cursor, size);
			m_cursor += sizeof(T) * size;
			return result;
		}

		template<typename T>
		queue<T> allocateQueue(int size)
		{
			T* data = (T*)(m_data + m_cursor);
			queue<T> result(data, size);
			m_cursor += sizeof(T) * size;
			return result;
		}

		template<typename T>
		void allocateQueues(queue<T>* outQueues, int queueSize, int queueCount)
		{
			T* data = (T*)(m_data + m_cursor);
			for (int i = 0; i < queueSize; i++)
			{
				outQueues[i].m_data = data + i * queueSize;
				outQueues[i].m_size = queueSize;
				outQueues[i].m_firstCursor = 0;
				outQueues[i].m_lastCursor = 0;
			}
			m_cursor += sizeof(T) * queueSize * queueCount;
		}

	private:
		char* m_data;
		size_t m_cursor;
		size_t m_size;
		size_t* m_stack;
	};
}