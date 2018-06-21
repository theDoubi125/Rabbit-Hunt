#include "editor_world.h"

#include "imgui/imgui.h"

namespace editor
{
	namespace world
	{
		void drawWorld(const character::manager& characterManager)
		{
			static float zoom = 2;
			static float radiuses = 10;
			static vec2 offset(radiuses, radiuses);
			vec2 positions[MAX_CHARACTER_COUNT];
			character::handle handles[MAX_CHARACTER_COUNT];
			characterManager.getHandles(handles, MAX_CHARACTER_COUNT);
			characterManager.get(handles, positions, characterManager.count());
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1, 1, 1, 1));
			ImGui::Begin("World", nullptr, ImGuiWindowFlags_NoTitleBar);
			for (int i = 0; i < characterManager.count(); i++)
			{
				ImVec2 pos(positions[i].x * zoom + ImGui::GetWindowPos().x + offset.x, positions[i].y * zoom + ImGui::GetWindowPos().y + offset.y);
				ImGui::GetWindowDrawList()->AddCircle(pos, radiuses, 0xff000000);
			}
			ImGui::End();
			ImGui::PopStyleColor();
			ImGui::PopStyleVar();
		}
	}
}