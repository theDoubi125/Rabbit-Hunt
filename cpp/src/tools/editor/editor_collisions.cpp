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

	void drawCollisionHandlerEditor()
	{
		static collision::Circle A = { vec2(50, 50), 30 };
		static collision::Circle B = { vec2(70, 70), 30 };

		static vec2 velocityA = { 0, 0 };
		static vec2 velocityB = { 0, 0 };

		static float cellSize = 50;

		ImGui::DragFloat("Cell Size", &cellSize, 0.1f, 1);

		ImGui::DragFloat2("A pos : ", (float*)(&A.pos));
		ImGui::DragFloat2("B pos : ", (float*)(&B.pos));
		ImGui::DragFloat("A radius : ", (float*)(&A.r), 0, 0, 100.0f);
		ImGui::DragFloat("B radius : ", (float*)(&B.r), 0, 0, 100.0f);


		ImGui::DragFloat2("A velocity : ", (float*)(&velocityA), 0.1f, -50, 50);
		ImGui::DragFloat2("B velocity : ", (float*)(&velocityB), 0.1f, -50, 50);

		ImGui::BeginChild("Draw zone", vec2(0, 0), true);
		ImVec2 pos = ImGui::GetCursorScreenPos();
		ImDrawList* drawList = ImGui::GetWindowDrawList();

		for (int i = 0; i < 20; i++)
		{
			for (int j = 0; j < 20; j++)
			{
				drawList->AddRect(pos + vec2(i * cellSize, j * cellSize), pos + vec2((i + 1) * cellSize, (j + 1) * cellSize), ImColor(255, 255, 255, 120));
			}
		}
		drawList->AddCircle(pos + A.pos, A.r, ImColor(255, 0, 0), 30);
		drawList->AddCircle(pos + B.pos, B.r, ImColor(0, 255, 0), 30);
		drawList->AddLine(pos + A.pos, pos + B.pos, ImColor(255, 255, 255, 100));

		drawList->AddLine(pos + A.pos, pos + A.pos + velocityA * cellSize, ImColor(255, 0, 0));
		drawList->AddLine(pos + B.pos, pos + B.pos + velocityB * cellSize, ImColor(0, 255, 0));

		drawList->AddLine(pos + A.pos, pos + A.pos + vec2(velocityA.x * cellSize, 0), ImColor(255, 0, 0, 100), 2);
		drawList->AddLine(pos + A.pos, pos + A.pos + vec2(0, velocityA.y * cellSize), ImColor(255, 0, 0, 100), 2);
		drawList->AddLine(pos + B.pos, pos + B.pos + vec2(velocityB.x * cellSize, 0), ImColor(0, 255, 0, 100), 2);
		drawList->AddLine(pos + B.pos, pos + B.pos + vec2(0, velocityB.y * cellSize), ImColor(0, 255, 0, 100), 2);

		vec2 C = pos + (A.pos + B.pos) / 2;
		float utilVelocityAx = velocityA.x * (B.pos.x - A.pos.x);
		float utilVelocityAy = velocityA.y * (B.pos.y - A.pos.y);
		float utilVelocityBx = velocityB.x * -(B.pos.x - A.pos.x);
		float utilVelocityBy = velocityB.y * -(B.pos.y - A.pos.y);

		drawList->AddLine(C, C + vec2(utilVelocityAx - utilVelocityBx, utilVelocityAy - utilVelocityBy), ImColor(255, 255, 0, 100), 3);

		ImGui::EndChild();
	}

	void drawCollisionDetectEditor()
	{
		static collision::AABB A = { { 50, 50 },{ 60, 60 } };
		static collision::AABB B = { { 10, 10 },{ 55, 55 } };
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
		collision::BitArray flags[] = { 0, 0 };
		collision::testAABBCollisions(&A, &B, flags, 1);
		ImGui::Text("%d", flags[0]);

		static collision::Circle CA = { { 50, 50 }, 20 };
		static collision::Circle CB = { { 60, 60 }, 10 };

		ImGui::DragFloat3("CA : ", (float*)(&CA), 0.5f, 0, 100.0f);
		ImGui::DragFloat3("CB : ", (float*)(&CB), 0.5f, 0, 100.0f);

		drawList->AddCircle(pos + CA.pos, CA.r, 0xffffff00);
		drawList->AddCircle(pos + CB.pos, CB.r, 0xffff00ff);
		flags[0] = 0;

		collision::testCircleCollisions(&CA, &CB, flags, 1);

		ImGui::Text("%d", flags[0]);

		collision::Circle Cs[] = { CA, CB };
		collision::AABB AABBs[] = { A, B };

		collision::testAABBCircleCollisions(AABBs, Cs, flags, 2);

		ImGui::Text("%d %d", flags[0], flags[1]);
	}
	void drawCollisionEditor()
	{
		ImGui::Begin("Collision Editor");
		if (ImGui::CollapsingHeader("Detection"))
		{
			drawCollisionDetectEditor();
		}
		if (ImGui::CollapsingHeader("Resolution"))
		{
			drawCollisionHandlerEditor();
		}

		


		ImGui::End();
	}
}