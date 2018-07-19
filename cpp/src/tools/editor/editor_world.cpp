#include "editor_world.h"
#include <iostream>

#include "memory/allocator.h"
#include "imgui/imgui.h"
#include "path/dijkstra.h"
#include "path/path.h"

namespace editor
{
	namespace world
	{
		char* editorToolNames[] = { "Window", "Wall", "Path" };

		static memory::allocator allocator(1000000);

		ivec2 screenToWorld(const level::accessibilityMap& map, ivec2 pos, float cellSize)
		{
			vec2 windowPos = ImGui::GetWindowPos();
			return toIntVec((toFloatVec(pos) - windowPos - toFloatVec(map.min)) / cellSize);
		}
		
		ivec2 worldToScreen(const level::accessibilityMap& map, ivec2 pos, float cellSize)
		{
			vec2 windowPos = ImGui::GetWindowPos();
			return toIntVec(pos * cellSize) + map.min + toIntVec(windowPos);
		}

		void drawMouseCursor(level::accessibilityMap& map, float cellSize)
		{
			vec2 mousePos = ImGui::GetMousePos();
			ivec2 mouseCell = screenToWorld(map, toIntVec(mousePos), cellSize);
			ivec2 A = worldToScreen(map, mouseCell, cellSize);

			ImDrawList* drawList = ImGui::GetWindowDrawList();
			ivec2 B = A + ivec2((int)cellSize, (int)cellSize);
			drawList->AddRectFilled(toFloatVec(A), toFloatVec(B), 0x55ffffff);
		}

		void drawCharacters(const character::manager& characterManager, const level::accessibilityMap& map, int cellSize)
		{
			float radiuses = (float)cellSize / 2.0f;
			vec2 offset(radiuses, radiuses);
			vec2 windowPos = ImGui::GetWindowPos();
			vec2 positions[MAX_CHARACTER_COUNT];
			character::handle handles[MAX_CHARACTER_COUNT];
			characterManager.getHandles(handles, MAX_CHARACTER_COUNT);
			characterManager.get(handles, positions, characterManager.count());
			ImDrawList* drawList = ImGui::GetWindowDrawList();

			for (int i = 0; i < characterManager.count(); i++)
			{
				ImVec2 pos(positions[i].x * cellSize + windowPos.x + offset.x, positions[i].y * cellSize + windowPos.y + offset.y);
				drawList->AddCircle(pos, radiuses, 0xffffffff);
			}
		}

		void updateWallTool(level::accessibilityMap& map, editorData& data)
		{
			vec2 mousePos = ImGui::GetMousePos();
			ivec2 mouseCell = screenToWorld(map, toIntVec(mousePos), (float)data.cellSize);
			const ImGuiIO& IO = ImGui::GetIO();
			if (IO.MouseDown[0])
			{
				map.setAccessible(mouseCell, !data.selectedToolData.wall.selectedTile);
			}
		}

		void updatePathTool(editorData& data, const character::manager& characterManager, action::scheduler::manager& scheduler, level::accessibilityMap& map)
		{
			const ImGuiIO& IO = ImGui::GetIO();

			vec2 mousePos = ImGui::GetMousePos();
			ivec2 mouseCell = screenToWorld(map, toIntVec(mousePos), (float)data.cellSize);

			vec2 positions[MAX_CHARACTER_COUNT];
			character::handle handles[MAX_CHARACTER_COUNT];
			characterManager.getHandles(handles, MAX_CHARACTER_COUNT);
			characterManager.get(handles, positions, characterManager.count());

			if (IO.MouseClicked[0] && map.isAccessible(mouseCell))
			{
				
				bool onCharacterCell = false;
				for (int i = 0; i < characterManager.count(); i++)
				{
					if (toIntVec(positions[i]) == mouseCell)
					{
						onCharacterCell = true;
						data.selectedToolData.path.selectedCharacter = handles[i];
					}
				}
				if (!onCharacterCell && data.selectedToolData.path.selectedCharacter.id >= 0)
				{
					float radiuses = (float)data.cellSize / 2.0f;
					vec2 offset(radiuses, radiuses);

					allocator.pushStack();
					character::handle selectedCharacter = data.selectedToolData.path.selectedCharacter;
					vec2 characterPos = characterManager.get(selectedCharacter);
					path::dijkstra::dijkstraMap dijkstraMap = path::dijkstra::dijkstra(toIntVec(characterPos), 40, allocator, map);
					path::path currentPath;
					currentPath.steps = allocator.allocateQueue<path::step>(50);
					path::dijkstra::getPathTo(dijkstraMap, mouseCell, currentPath, allocator);
					if (currentPath.steps.size() > 0)
					{
						queue<action::typedActionData> actions = allocator.allocateQueue<action::typedActionData>(100);
						path::toActionLists(&currentPath.steps, &actions, 1, 1);
						action::typedActionData actionData;
						while (actions.size() > 0)
						{
							actionData = actions.dequeue();
							scheduler.addAction(selectedCharacter, actionData);

						}
					}
					allocator.popStack();
				}
			}
			if (data.selectedToolData.path.selectedCharacter.id >= 0)
			{
				ImDrawList* drawList = ImGui::GetWindowDrawList();
				float radiuses = (float)data.cellSize / 2.0f;
				vec2 offset(radiuses, radiuses);

				for (int i = 0; i < characterManager.count(); i++)
				{
					if (handles[i] == data.selectedToolData.path.selectedCharacter)
					{
						vec2 windowPos = ImGui::GetWindowPos();
						ImVec2 pos(positions[i].x * data.cellSize + windowPos.x + offset.x, positions[i].y * data.cellSize + windowPos.y + offset.y);
						drawList->AddCircleFilled(pos, radiuses / 5, 0xff7777ff);

						ivec2 characterCell = toIntVec(positions[i]);
						int maxDist = 20;
						allocator.pushStack();
						path::dijkstra::dijkstraMap dijkstraMap = path::dijkstra::dijkstra(characterCell, maxDist, allocator, map);
						path::path currentPath;
						currentPath.steps = allocator.allocateQueue<path::step>(50);
						path::dijkstra::getPathTo(dijkstraMap, mouseCell, currentPath, allocator);
						ivec2* pathBuffer = allocator.allocate<ivec2>(50);
						pathBuffer[0] = characterCell;
						int pathLength = currentPath.steps.size();
						ivec2 cursor = characterCell;
						for (int i = 0; i < pathLength; i++)
						{
							path::step step = currentPath.steps.dequeue();
							cursor = cursor + path::getDirectionVector(step.lookDirection) * step.distance;
							pathBuffer[i] = cursor;
						}
						path::sortByXAndY(pathBuffer, pathBuffer, pathLength);

						for (int i = 0; i < pathLength; i++)
						{
							ivec2 A = worldToScreen(map, pathBuffer[i], (float)data.cellSize);

							ivec2 B = A + ivec2((int)data.cellSize, (int)data.cellSize);
							drawList->AddRectFilled(toFloatVec(A), toFloatVec(B), 0x9900ffff);
						}

						allocator.popStack();
					}
				}

				
			}
		}

		void updateTool(editorData& data, const character::manager& characterManager, action::scheduler::manager& scheduler, level::accessibilityMap& map)
		{
			switch (data.selectedTool)
			{
			case editorTool::Path:
				updatePathTool(data, characterManager, scheduler, map);
				break;
			case editorTool::Wall:
				updateWallTool(map, data);
				break;
			}
		}

		void drawPathToolEditor(editorData& data)
		{
			ImGui::Text("Selected character : %d", data.selectedToolData.path.selectedCharacter.id);
		}

		void updateWallToolEditor(editorData& data)
		{
			ImGui::Checkbox("Add wall", &data.selectedToolData.wall.selectedTile);
		}

		void drawToolEditor(editorData& data)
		{
			switch (data.selectedTool)
			{
			case editorTool::Path:
				drawPathToolEditor(data);
				break;
			case editorTool::Wall:
				updateWallToolEditor(data);
				break;
			}
		}

		void drawToolSelector(editorData& data)
		{
			if (ImGui::Begin("Tool Selector"))
			{
				if (ImGui::BeginCombo("Tool", editorToolNames[(int)data.selectedTool]))
				{
					for (int i = 0; i < (int)editorTool::Last; i++)
					{
						if (ImGui::Selectable(editorToolNames[i], i == (int)data.selectedTool))
						{
							data.selectedTool = (editorTool)i;
						}
					}
					ImGui::EndCombo();
				}

				drawToolEditor(data);

				ImGui::End();
			}
		}

		void drawWorldEditor(editorData& data, const character::manager& characterManager, action::scheduler::manager& scheduler, level::accessibilityMap& map)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1, 1, 1, 1));
			ImGuiWindowFlags flags = 0;
			if (data.selectedTool != editorTool::Window)
				flags = ImGuiWindowFlags_NoInputs;
			if (ImGui::Begin("World", nullptr, ImGuiWindowFlags_NoTitleBar | flags))
			{
				ImDrawList* drawList = ImGui::GetWindowDrawList();
				ImVec2 windowPos = ImGui::GetWindowPos();
				ImVec2 mousePos = ImGui::GetMousePos();

				for (int i = 0; i < map.size.x; i++)
				{
					for (int j = 0; j < map.size.y; j++)
					{
						ivec2 pos = (map.min + ivec2(i, j)) * data.cellSize;
						ivec2 max = pos + ivec2(data.cellSize, data.cellSize);
						int color = 0xffff0000;

						ImVec2 A, B;
						A.x = pos.x + windowPos.x;
						B.x = max.x + windowPos.x;
						A.y = pos.y + windowPos.y;
						B.y = max.y + windowPos.y;
						if (map.isAccessible(ivec2(i, j)))
							color = 0xffff0000;
						else color = 0xff0000ff;
							drawList->AddRectFilled(A, B, color);
					}
				}
				drawCharacters(characterManager, map, data.cellSize);
				drawMouseCursor(map, (float)data.cellSize);
				updateTool(data, characterManager, scheduler, map);
				ImGui::End();
			}
			ImGui::PopStyleColor();
			ImGui::PopStyleVar();
			drawToolSelector(data);
		}
	}
}