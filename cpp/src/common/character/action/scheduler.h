#pragma once
#include "util/buffer.h"
#include "character/character.h"
#include "common.h"
#include "util/buffer.h"
#include "dispatcher.h"
#include "memory/allocator.h"

#define MAX_ACTION_COUNT 50

namespace action
{

	namespace scheduler
	{
		class manager
		{
		public:
			manager(memory::allocator& allocator, int size);
			void addAction(character::handle character, typedActionData action);

			void dequeueNextAction(const character::handle* characters, typedActionData* outActions, int count);

		public:
			queue<typedActionData> m_actionQueues[MAX_CHARACTER_COUNT];
			int m_actionQueuesCount;
		};
	}
}