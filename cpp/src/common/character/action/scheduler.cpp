#include "scheduler.h"

namespace action
{
	extern std::string typeNames[50] =
	{
		"none",
		"idle",
		"moving",
	};

	namespace scheduler
	{
		using namespace memory::util;

		void manager::allocate(int queuesCount)
		{
			m_actionQueuesCount = queuesCount;
			allocateQueues<typedActionData>(m_actionQueues, MAX_CHARACTER_COUNT, queuesCount);
		}

		void manager::addAction(character::handle character, typedActionData action)
		{
			m_actionQueues[character.id].enqueue(action);
		}

		void manager::getCharactersExcept(const character::handle* exceptCharacters, character::handle* outCharacters, int exceptSize, int& outCharactersCount)
		{
			int exceptCursor = 0;
			for (int i = 0; i < MAX_CHARACTER_COUNT; i++)
			{
				while (exceptCursor < exceptSize && exceptCharacters[exceptCursor].id < i)
					exceptCursor++;
				if (exceptCursor < exceptSize && exceptCharacters[exceptCursor].id == i)
				{
					exceptCursor++;
					continue;
				}
				else if (m_actionQueues[i].size() > 0)
				{
					outCharacters[outCharactersCount].id = i;
					outCharactersCount++;
				}
			}
		}

		void manager::dequeueNextAction(const character::handle* inCharacters, character::handle* outCharacters, typedActionData* outActions, int inCount, int& outCount)
		{
			int outCursor = 0;
			for (int i = 0; i < inCount; i++)
			{
				if (m_actionQueues[inCharacters[i].id].size() > 0)
				{
					outCharacters[outCursor] = inCharacters[i];
					outActions[outCursor] = m_actionQueues[inCharacters[i].id].dequeue();
					outCount++;
				}
			}
		}
	}
}