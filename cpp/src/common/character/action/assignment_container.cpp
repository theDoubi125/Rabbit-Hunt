#include "assignment_container.h"
#include "util/sorted_array.h"

namespace action
{
	void assignmentContainer::add(character::handle character, int duration)
	{
		array::addSortedWithIndex(characters, durations, character, -duration, m_count);
	}

	void assignmentContainer::handleInput()
	{
		const assignedAction *inputCharacters = input.getData();
		for (int i = 0; i < input.size(); i++)
		{
			int insertIndex = array::getIndexForSortInsert(durations, -inputCharacters[i].data.duration, m_count);
			array::insert(durations, insertIndex, -inputCharacters[i].data.duration, m_count);
			array::insert(characters, insertIndex, inputCharacters[i].character, m_count);
			array::insert(actionData, insertIndex, inputCharacters[i].data, m_count);
			m_count++;
		}
		input.clear();
	}

	void assignmentContainer::update()
	{
		int firstToRemove = -1;
		for (int i = m_count - 1; i >= 0; i--)
		{
			durations[i]++;
			if (durations[i] >= 0)
			{
				firstToRemove = i;
			}
		}
		if (firstToRemove >= 0)
		{
			for (int i = 0; i < m_count - firstToRemove; i++)
				output.add(characters[i + firstToRemove]);
			m_count = firstToRemove;
		}
	}
}