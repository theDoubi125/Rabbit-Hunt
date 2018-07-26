#include "assignment_container.h"
#include "util/sorted_array.h"

namespace action
{

	void assignmentContainer::handleInput()
	{
		const assignedAction *inputCharacters = input.getData();
		for (int i = 0; i < input.size(); i++)
		{
			int insertIndex = array::getIndexForSortInsert(durations, -inputCharacters[i].data.duration, count);
			array::insert(durations, insertIndex, -inputCharacters[i].data.duration, count);
			array::insert(characters, insertIndex, inputCharacters[i].character, count);
			array::insert(actionData, insertIndex, inputCharacters[i].data, count);
			count++;
		}
		input.clear();
	}

	void assignmentContainer::update()
	{
		int firstToRemove = -1;
		for (int i = count - 1; i >= 0; i--)
		{
			durations[i]++;
			if (durations[i] >= 0)
			{
				firstToRemove = i;
			}
		}
		if (firstToRemove >= 0)
		{
			for (int i = 0; i < count - firstToRemove; i++)
				output.add(characters[i + firstToRemove]);
			count = firstToRemove;
		}
	}
}