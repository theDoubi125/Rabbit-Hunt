#include "allocator.h"

namespace memory
{
	namespace util
	{
		allocator* mainAllocator = nullptr;

		void createMainAllocator(size_t size)
		{
			mainAllocator = new allocator(size);
		}
	}
}