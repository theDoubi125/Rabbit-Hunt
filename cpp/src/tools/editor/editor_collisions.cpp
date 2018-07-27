#include "editor_container.h"
#include "imgui/imgui.h"
#include "editor_main.h"
#include "util/packed_array.h"
#include "util/sorted_array.h"
#include "imgui/imgui_internal.h"

#include <cstdio>
#include <vector>
#include "collision.h"
#include "editor_util.h"

using namespace ImGui;

namespace editor
{
	void drawCollisionEditor()
	{
		static collision::AABB A = { {50, 50}, {60, 60} };
		static collision::AABB B = { { 10, 10}, {55, 55} };
		ImGui::Begin("Collision Editor");
		vec2 ASize = A.max - A.min;
		vec2 BSize = B.max - B.min;
		ImGui::DragFloat2("A pos : ", (float*)(&A), 0.5f, 0, 100.0f);
		ImGui::DragFloat2("A Size : ", (float*)(&ASize), 0.5f, 0, 100.0f);
		ImGui::DragFloat2("B : ", (float*)(&B), 0.5f, 0, 100.0f);
		ImGui::DragFloat2("B Size : ", (float*)(&BSize), 0.5f, 0, 100.0f);

		A.max = A.min + ASize;
		B.max = B.min + BSize;

		ImDrawList* drawList = ImGui::GetWindowDrawList();
		ImGuiWindow* window = GetCurrentWindow();
		vec2 pos = window->DC.CursorPos;
		drawList->AddRect(pos + A.min, pos + A.max, 0xffffff00);
		drawList->AddRect(pos + B.min, pos + B.max, 0xffff00ff);
		collision::BitArray flags = 0;
		collision::testAABBCollisions(&A, &B, &flags, 1);
		ImGui::Text("%d", flags);

		static collision::Circle CA = { { 50, 50 }, 20 };
		static collision::Circle CB = { { 60, 60 }, 10 };
		ImGui::DragFloat2("CA : ", (float*)(&CA), 0.5f, 0, 100.0f);
		ImGui::DragFloat3("CB : ", (float*)(&CB.pos), 0.5f, 0, 100.0f);

		drawList->AddCircle(pos + CA.pos, CA.r, 0xffffff00);
		drawList->AddCircle(pos + CB.pos, CB.r, 0xffff00ff);
		flags = 0;

		collision::testCircleCollisions(&CA, &CB, &flags, 1);
		ImGui::Text("%d", flags);

		ImGui::End();
	}
}