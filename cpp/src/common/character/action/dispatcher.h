#pragma once
#include "util/buffer.h"
#include "character\character.h"
#include <map>
#include "common.h"
#include "scheduler.h"

#define MAX_ACTION_COUNT 50

namespace action
{
	namespace scheduler
	{
		class manager;
	}

	namespace dispatcher
	{
		namespace tables
		{
			struct BusyCharacters
			{
				character::handle characters[MAX_CHARACTER_COUNT];
				int busyDurations[MAX_CHARACTER_COUNT];
				int count = 0;
			};

			struct IdleCharacters
			{
				character::handle characters[MAX_CHARACTER_COUNT];
				int count = 0;
			};
		}
		class manager
		{
		public:
			manager();
			void bindHandler(type actionType, buffer<assignedAction>& handlerInput);
			void update(scheduler::manager& m_scheduler);

			tables::BusyCharacters busyTable;
			tables::IdleCharacters idleTable;

		private:
			buffer<assignedAction> m_actionHandlerInputs[type::LAST];
			int* m_actionDurations[type::LAST];
		};
	}
}