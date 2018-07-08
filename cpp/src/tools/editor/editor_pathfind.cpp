#include "editor_pathfind.h"
#include "path/dijkstra.h"
#include "memory/allocator.h"
#include "imgui/imgui.h"
#include "editor_main.h"

namespace editor
{
	namespace pathfind
	{
		int compare(const ivec2& A, const ivec2& B)
		{
			int result = A.y - B.y;
			if (result == 0)
				result = A.x - B.x;
			return result;
		}

		void sortByXAndY(ivec2* tBase, ivec2* t, int count)
		{
			if (count >= 2)
			{
				//ImGui::Text("sort between %d and %d", (int)(t - tBase), (int)(t - tBase) + count);
				ivec2 pivot = t[count-1];
				//ImGui::Text("pivot : (%d, %d)", pivot.x, pivot.y);
				int leftCursor = 0, rightCursor = count - 1;
				ivec2 buf;
				for (int i = 0; i < count - 1; i++)
				{
					if (compare(t[i], pivot) <= 0) // swap this element with the last one greater than the pivot
					{
						buf = t[i];
						t[i] = t[leftCursor];
						t[leftCursor] = buf;
						leftCursor++;
					}
				}
				t[count-1] = t[leftCursor];
				t[leftCursor] = pivot;

				/*for (ivec2* i = tBase; i < t + count; i++)
				{
					if (i != tBase)
						ImGui::SameLine();
					if (i >= t)
					{
						if (*i == pivot)
							ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0, 0, 1));
						else
							ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0.5f, 0.5f, 1));
						ImGui::Text("(%d, %d)", i->x, i->y);
						ImGui::PopStyleColor();
					}
					else
						ImGui::Text("(%d, %d)", i->x, i->y);
				}*/
				sortByXAndY(tBase, t, leftCursor);
				if(leftCursor > 0)
					sortByXAndY(tBase, t + leftCursor, count - leftCursor);
				else sortByXAndY(tBase, t + 1, count - 1);
			}
		}

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
			for (int i = 0; i < pathLength; i++)
			{
				pathBuffer[i] = currentPath.steps.dequeue().movement;
			}
			sortByXAndY(pathBuffer, pathBuffer, pathLength);
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