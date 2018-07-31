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
			typedActionData nextActionsBuffer[MAX_CHARACTER_COUNT];
			character::handle characters[MAX_CHARACTER_COUNT];
			int charactersCount = 0;

			scheduler.getCharactersExcept(busyTable.characters, characters, busyTable.count, charactersCount);

			character::handle charactersWithAction[MAX_CHARACTER_COUNT];
			int charactersWithActionCount = 0;
			scheduler.dequeueNextAction(characters, charactersWithAction, nextActionsBuffer, charactersCount, charactersWithActionCount);
			int cursors[type::LAST];
			memset(cursors, 0, type::LAST * sizeof(int));
			
			for (int i = 0; i < charactersWithActionCount; i++)
			{
				type actionType = nextActionsBuffer[i].action;
				m_actionHandlerInputs[actionType].add({ charactersWithAction[i], nextActionsBuffer[i].data });
			}
		}
	}
}