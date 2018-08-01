#include "dispatcher.h"
#include "util/sorted_array.h"

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
				int index = array::getIndexForSortInsert(busyTable.busyDurations, -nextActionsBuffer[i].data.duration, busyTable.count);
				// sort by -duration to have lowest durations on the right
				array::insert(busyTable.busyDurations, index, -nextActionsBuffer[i].data.duration, busyTable.count);
				array::insert(busyTable.characters, index, charactersWithAction[i], busyTable.count);
				busyTable.count++;
			}

			for (int i = 0; i < busyTable.count; i++)
			{
				busyTable.busyDurations[i]++;
			}

			for (; busyTable.count > 0 && busyTable.busyDurations[busyTable.count - 1] >= 0; busyTable.count--); // remove 0s
		}
	}
}