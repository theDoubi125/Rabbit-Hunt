#include "dispatcher.h"

namespace action
{
	namespace dispatcher
	{
		manager::manager()
		{

		}

		void manager::bindHandler(type actionType, buffer<assignedAction>& handlerInput)
		{
			m_actionHandlerInputs[actionType] = handlerInput;
		}

		void manager::update(scheduler::manager& scheduler)
		{
			actionData nextActionsBuffer[MAX_CHARACTER_COUNT];
			const character::handle* characters = m_unassignedCharacters.getData();
			
			scheduler.dequeueNextAction(characters, nextActionsBuffer, m_unassignedCharacters.size());
			int cursors[type::LAST];
			memset(cursors, 0, type::LAST * sizeof(int));
			for (int i = 0; i < m_unassignedCharacters.size(); i++)
			{
				type actionType = nextActionsBuffer[i].action;
				m_actionHandlerInputs[actionType].add({ characters[i], nextActionsBuffer[i].duration, nextActionsBuffer[i].direction });
			}
			m_unassignedCharacters.clear();
		}
	}
}