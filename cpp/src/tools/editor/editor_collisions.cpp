#include "editor_container.h"
#include "imgui/imgui.h"
#include "editor_main.h"
#include "util/packed_array.h"
#include "util/sorted_array.h"

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
		ImGui::DragFloat4("A : ", (float*)(&A), 0.5f, 0, 100.0f);
		ImGui::DragFloat4("B : ", (float*)(&B), 0.5f, 0, 100.0f);

		collision::BitArray flags = 0;
		collision::testAABBCollisions(&A, &B, &flags, 1);
		ImGui::Text("%d", flags);
		ImGui::End();
	}
}