#include "editor_main.h"
#include "imgui/imgui.h"
#include "editor_container.h"
#include "editor_actions.h"
#include "editor_dispatcher.h"
#include "editor_pathfind.h"
#include "character/action/assignment_container.h"

namespace editor
{
	bool showEditor = true;

	bool pArraySelected = false;
	bool aArraySelected = false;
	bool testSelected = false;
	bool idleSelected = false;
	bool demoWindowSelected = false;
	bool moveSelected = false;
	bool dispatcherSelected = false;
	bool pathfindSelected = true;

	void drawEditor(float deltaTime)
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Container"))
			{
				ImGui::MenuItem("Packed Array", NULL, &pArraySelected);
				ImGui::MenuItem("Alloc Array", NULL, &aArraySelected);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Actions"))
			{
				ImGui::MenuItem("Idle Action", NULL, &idleSelected);
				ImGui::MenuItem("Move Action", NULL, &moveSelected);
				ImGui::MenuItem("Dispatcher", NULL, &dispatcherSelected);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Demo"))
			{
				ImGui::MenuItem("Show Demo Window", NULL, &demoWindowSelected);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Pathfind"))
			{
				ImGui::MenuItem("Dijkstra", NULL, &pathfindSelected);
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
		if (pArraySelected)
		{
			container::drawPackedArrayEditor();
		}
		if (aArraySelected)
		{
			container::drawAllocArrayEditor();
		}
		if (idleSelected)
		{
			static action::assignmentContainer idle;
			drawAssignmentEditor("Idle Editor", idle);
		}
		if (demoWindowSelected)
		{
			ImGui::ShowDemoWindow(&demoWindowSelected);
		}
		if (moveSelected)
		{
			drawMoveEditor();
		}
		if (dispatcherSelected)
		{
			dispatcher::drawDispatcherEditor(deltaTime);
		}
		if (pathfindSelected)
		{
			pathfind::drawDijkstraEditor();
		}
	}
}