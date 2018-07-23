#include "path_tool.h"
#include "memory/allocator.h"
#include "imgui/imgui.h"
#include "path/dijkstra.h"

using namespace memory::util;

namespace editor
{
	namespace tools
	{
		namespace pathTool
		{
			void updatePathTool(world::editorData& data, const character::manager& characterManager, action::scheduler::manager& scheduler, level::accessibilityMap& map)
			{
				const ImGuiIO& IO = ImGui::GetIO();

				vec2 mousePos = ImGui::GetMousePos();
				ivec2 mouseCell = world::screenToWorld(map, toIntVec(mousePos), (float)data.cellSize);

				vec2 positions[MAX_CHARACTER_COUNT];
				character::handle handles[MAX_CHARACTER_COUNT];
				characterManager.getHandles(handles, MAX_CHARACTER_COUNT);
				characterManager.get(handles, positions, characterManager.count());

				if (IO.MouseClicked[0] && map.isAccessible(mouseCell))
				{

					bool onCharacterCell = false;
					for (int i = 0; i < characterManager.count(); i++)
					{
						if (roundVecToInt(positions[i]) == mouseCell)
						{
							onCharacterCell = true;
							data.selectedToolData.path.selectedCharacter = handles[i];
						}
					}
					if (!onCharacterCell && data.selectedToolData.path.selectedCharacter.id >= 0)
					{
						float radiuses = (float)data.cellSize / 2.0f;
						vec2 offset(radiuses, radiuses);

						pushAllocatorStack();
						character::handle selectedCharacter = data.selectedToolData.path.selectedCharacter;
						vec2 characterPos = characterManager.get(selectedCharacter);
						path::dijkstra::dijkstraMap dijkstraMap = path::dijkstra::dijkstra(roundVecToInt(characterPos), 40, map);
						path::path currentPath;
						currentPath.steps = mainAllocator->allocateQueue<path::step>(50);
						path::dijkstra::getPathTo(dijkstraMap, mouseCell, currentPath);
						if (currentPath.steps.size() > 0)
						{
							queue<action::typedActionData> actions = mainAllocator->allocateQueue<action::typedActionData>(100);
							path::toActionLists(&currentPath.steps, &actions, 1, 1 / 60.0f);
							action::typedActionData actionData;
							while (actions.size() > 0)
							{
								actionData = actions.dequeue();
								scheduler.addAction(selectedCharacter, actionData);

							}
						}
						popAllocatorStack();
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

							ivec2 characterCell = roundVecToInt(positions[i]);
							int maxDist = 20;
							pushAllocatorStack();
							path::dijkstra::dijkstraMap dijkstraMap = path::dijkstra::dijkstra(characterCell, maxDist, map);
							path::path currentPath;
							currentPath.steps = mainAllocator->allocateQueue<path::step>(50);
							path::dijkstra::getPathTo(dijkstraMap, mouseCell, currentPath);
							ivec2* pathBuffer = mainAllocator->allocate<ivec2>(50);
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
								ivec2 A = world::worldToScreen(map, pathBuffer[i], (float)data.cellSize);

								ivec2 B = A + ivec2((int)data.cellSize, (int)data.cellSize);
								drawList->AddRectFilled(toFloatVec(A), toFloatVec(B), 0x9900ffff);
							}

							popAllocatorStack();
						}
					}


				}
			}
		}
	}
}