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

		void manager::dequeueNextAction(const character::handle* characters, typedActionData* outActions, int count)
		{
			for (int i = 0; i < count; i++)
			{
				if(m_actionQueues[characters[i].id].size() > 0)
					outActions[i] = m_actionQueues[characters[i].id].dequeue();
				else outActions[i].action = action::type::NONE;
			}
		}
	}
}