#include "editor_util.h"
#include "imgui/imgui.h"

namespace ImGui
{
	char* getDirectionName(const action::direction& direction)
	{
		static char up[] = "up";
		static char down[] = "down";
		static char left[] = "left";
		static char right[] = "right";
		switch (direction)
		{
		case action::direction::UP:
			return up;
		case action::direction::DOWN:
			return down;
		case action::direction::LEFT:
			return left;
		case action::direction::RIGHT:
			return right;
		default:
			return nullptr;
		}
	}

	bool DirectionSelector(char* label, action::direction* direction)
	{
		char* directionName = getDirectionName(*direction);
		bool result = false;
		if (BeginCombo(label, directionName))
		{
			for (int i = 0; i < 4; i++)
			{
				directionName = getDirectionName((action::direction)i);
				if (Selectable(directionName, *direction == i))
				{
					*direction = (action::direction)i;
					result = true;
				}
			}
			EndCombo();
		}
		return result;
	}
}