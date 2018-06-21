#pragma once
#include "util/buffer.h"
#include "character/character.h"
#include "common.h"
#include "util/buffer.h"
#include "dispatcher.h"

#define MAX_ACTION_COUNT 50

namespace action
{

	namespace scheduler
	{
		class manager
		{
		public:
			void addAction(character::handle character, actionData action);

			void dequeueNextAction(const character::handle* characters, actionData* outActions, int count);

		public:
			queue<actionData, MAX_ACTION_COUNT> m_actionQueues[MAX_CHARACTER_COUNT];
		};
	}
}