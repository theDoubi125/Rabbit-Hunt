#include "editor_pathfind.h"
#include "path/dijkstra.h"
#include "memory/allocator.h"
#include "imgui/imgui.h"
#include "editor_main.h"

namespace editor
{
	namespace pathfind
	{
		void drawDijkstraEditor()
		{
			static bool initialized = false;
			static char buffer[100];
			static ivec2 startPos = ivec2(0, 0);
			static ivec2 targetPos = ivec2(5, 5);
			static int maxDist = 10;
			static level::accessibilityMap accessibility;
			static memory::allocator allocator(1000000);
			static ivec2 target(3, 0);
			const unsigned size = 5000;
			const unsigned maxValue = 100;
			static ivec2 testModel[size];

			if (!initialized)
			{
				accessibility.min = ivec2(-5, -5);
				accessibility.size = ivec2(11, 11);
				accessibility.data = allocator.allocate<unsigned int>(accessibility.size.x * accessibility.size.y);
				accessibility.setAllAccessible();
				initialized = true; 
			}
			allocator.pushStack();
			path::dijkstra::dijkstraMap map = path::dijkstra::dijkstra(startPos, maxDist, allocator, accessibility);
			path::path currentPath;
			currentPath.steps = allocator.allocateQueue<path::step>(50);
			path::dijkstra::getPathTo(map, target, currentPath, allocator);
			ivec2* pathBuffer = allocator.allocate<ivec2>(50);
			pathBuffer[0] = startPos;
			int pathLength = currentPath.steps.size();
			ivec2 cursor = startPos;
			for (int i = 0; i < pathLength; i++)
			{
				path::step step = currentPath.steps.dequeue();
				cursor = cursor + path::getDirectionVector(step.lookDirection) * step.distance;
				pathBuffer[i] = cursor;
			}
			path::sortByXAndY(pathBuffer, pathBuffer, pathLength);
			if (ImGui::Begin("Dijkstra", &pathfindSelected, ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::DragInt2("Origin", (int*)&startPos, 0.1f);
				ImGui::DragInt2("Target", (int*)&target, 0.1f);
				int pathCursor = 0;
				for (int j = map.min.y; j < map.min.y + map.size.y; j++)
				{
					for (int i = map.min.x; i < map.min.x + map.size.x; i++)
					{
						if (i > map.min.y)
							ImGui::SameLine();
						float distance = map.distances[map.posToIndex(ivec2(i, j))];
						sprintf_s(buffer, 100, "Button %d %d", i, j);
						ImGui::PushID(buffer);
						if(distance >= 0)
							sprintf_s(buffer, 100, "%f", distance);
						else sprintf_s(buffer, 100, "??");
						if (pathCursor < pathLength && pathBuffer[pathCursor] == ivec2(i, j))
						{
							ImGui::PushStyleColor(ImGuiCol_Button, 0xff00ff00);
							pathCursor++;
						}
						else if (accessibility.isAccessible(ivec2(i, j)))
							ImGui::PushStyleColor(ImGuiCol_Button, 0xffff0000);
						else ImGui::PushStyleColor(ImGuiCol_Button, 0xff0000ff);
						if (ImGui::Button(buffer, { 30, 30 }))
						{
							accessibility.setAccessible(ivec2(i, j), !accessibility.isAccessible(ivec2(i, j)));
						}
						ImGui::PopStyleColor();
						ImGui::PopID();
					}
				}
				ImGui::End();
			}
			if (ImGui::Begin("Path Steps", &pathfindSelected, ImGuiWindowFlags_AlwaysAutoResize))
			{
				path::step steps[100];
				currentPath.steps.getData(steps, 100);
				for (int i = 0; i < pathLength; i++)
				{
					ImGui::Text("%d, %d", pathBuffer[i].x, pathBuffer[i].y);
				}
				ImGui::End();
			}
			allocator.popStack();
		}
	}
}