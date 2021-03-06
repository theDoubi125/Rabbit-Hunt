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
			void allocate(int size);
			void addAction(character::handle character, typedActionData action);

			void dequeueNextAction(const character::handle* inCharacters, character::handle* outCharacters, typedActionData* outActions, int inCount, int& outCount);
			void getCharactersExcept(const character::handle* exceptCharacters, character::handle* outCharacters, int exceptSize, int& outCharactersCount);

		public:
			queue<typedActionData> m_actionQueues[MAX_CHARACTER_COUNT];
			int m_actionQueuesCount;
		};
	}
}