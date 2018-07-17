#include "editor_world.h"

#include "imgui/imgui.h"

namespace editor
{
	namespace world
	{
		ivec2 screenToWorld(const level::accessibilityMap& map, ivec2 pos, float cellSize)
		{
			return toIntVec((pos - map.min) / cellSize);
		}
		
		ivec2 worldToScreen(const level::accessibilityMap& map, ivec2 pos, float cellSize)
		{
			return toIntVec(pos * cellSize) + map.min;
		}

		void drawWorldEditor(const character::manager& characterManager, const level::accessibilityMap& map)
		{
			static int cellSize = 20;
			static float radiuses = cellSize/2.0f;
			static vec2 offset(radiuses, radiuses);

			vec2 positions[MAX_CHARACTER_COUNT];
			character::handle handles[MAX_CHARACTER_COUNT];
			characterManager.getHandles(handles, MAX_CHARACTER_COUNT);
			characterManager.get(handles, positions, characterManager.count());
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1, 1, 1, 1));
			if (ImGui::Begin("World", nullptr, ImGuiWindowFlags_NoTitleBar))
			{

				ImDrawList* drawList = ImGui::GetWindowDrawList();
				ImVec2 windowPos = ImGui::GetWindowPos();
				ImVec2 mousePos = ImGui::GetMousePos();

				for (int i = 0; i < map.size.x; i++)
				{
					for (int j = 0; j < map.size.y; j++)
					{
						ivec2 pos = (map.min + ivec2(i, j)) * cellSize;
						ivec2 max = pos + ivec2(cellSize, cellSize);
						int color = 0xffff0000;

						ImVec2 A, B;
						A.x = pos.x + windowPos.x;
						B.x = max.x + windowPos.x;
						A.y = pos.y + windowPos.y;
						B.y = max.y + windowPos.y;
						if (A.x <= mousePos.x && A.y <= mousePos.y && B.x > mousePos.x && B.y > mousePos.y)
						{
							color = 0xff00ff00;
						}
						else if (map.isAccessible(ivec2(i, j)))
							color = 0xffff0000;
						else color = 0xff0000ff;
							drawList->AddRectFilled(A, B, color);
					}
				}
				for (int i = 0; i < characterManager.count(); i++)
				{
					ImVec2 pos(positions[i].x * cellSize + windowPos.x + offset.x, positions[i].y * cellSize + windowPos.y + offset.y);
					drawList->AddCircle(pos, radiuses, 0xffffffff);
				}
				ImGui::End();
			}
			ImGui::PopStyleColor();
			ImGui::PopStyleVar();
		}
	}
}