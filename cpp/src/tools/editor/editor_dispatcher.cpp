#include "editor_dispatcher.h"
#include "imgui/imgui.h"
#include "character/action/dispatcher.h"
#include "character/action/assignment_container.h"
#include "editor_util.h"
#include "transforms/move.h"
#include "editor/world/editor_world.h"
#include "editor/editor_actions.h"

using namespace ImGui;

using namespace action;
using Dispatcher = action::dispatcher::manager;
using Scheduler = action::scheduler::manager;
using Character = character::handle;

namespace editor
{
	namespace dispatcher
	{
		void drawDispatcherEditor(float deltaTime)
		{
			static Dispatcher dispatcherInstance;
			static Scheduler schedulerInstance;
			static character::manager characterManager;
			static assignmentContainer idleManager;
			static assignmentContainer noneManager;
			static action::move::moveAssignmentContainer movingManager;
			static level::accessibilityMap map;
			
			static typedActionData selectedAction;
			static Character selectedCharacter = { 0 };
			static bool initialized = false;

			static vec2 selectedPos;

			static bool isPlaying = false;
			static float autoPlayFps = 10;
			static float playTime = 0;

			static editor::world::editorData edData;

			if (!initialized)
			{
				idleManager.output = dispatcherInstance.m_unassignedCharacters.getRef();
				movingManager.output = dispatcherInstance.m_unassignedCharacters.getRef();
				noneManager.output = dispatcherInstance.m_unassignedCharacters.getRef();
				schedulerInstance.allocate(100);
				dispatcherInstance.bindHandler(type::IDLE, idleManager.input.getRef());
				dispatcherInstance.bindHandler(type::MOVING, movingManager.input.getRef());
				dispatcherInstance.bindHandler(type::NONE, noneManager.input.getRef());
				initialized = true; 
				map.min = ivec2(0, 0);
				map.size = ivec2(11, 11);
				map.data = memory::util::allocate<unsigned int>(map.size.x * map.size.y);
				map.setAllAccessible();
				map.setAccessible(ivec2(3, 3), false);
			}

			Begin("Dispatcher Editor");

			PushID("Dispatcher");

			char charBuffer[100];
			sprintf_s(charBuffer, 100, "%d", selectedCharacter.id);
			int intPos[2];
			intPos[0] = (int)selectedPos.x;
			intPos[1] = (int)selectedPos.y;
			if (ImGui::DragInt2("Position", intPos, 0.05f, -100, 100))
			{
				selectedPos.x = (float)intPos[0];
				selectedPos.y = (float)intPos[1];
			}
			if (Button("Add Character"))
			{
				selectedCharacter = characterManager.add(selectedPos);
			}
			character::handle handles[MAX_CHARACTER_COUNT];
			characterManager.getHandles(handles, MAX_CHARACTER_COUNT);
			vec2 positions[MAX_CHARACTER_COUNT];
			characterManager.get(handles, positions, characterManager.count());
			if (BeginCombo("Character Selector", charBuffer))
			{
				
				for (int i = 0; i < characterManager.count(); i++)
				{
					sprintf_s(charBuffer, 100, "%d", handles[i].id);
					if (Selectable(charBuffer, handles[i] == selectedCharacter))
					{
						selectedCharacter = handles[i];
					}
				}
				EndCombo();
			}

			if (BeginCombo("Action Type", typeNames[selectedAction.action].c_str()))
			{
				for (int i = 0; i < type::LAST; i++)
				{
					if (Selectable(typeNames[i].c_str(), i == selectedAction.action))
					{
						selectedAction.action = (type)i;
					}
				}
				EndCombo();
			}

			DirectionSelector("Direction", &selectedAction.data.direction);

			SliderInt("Action Duration", &selectedAction.data.duration, 0, 50);
			
			if (Button("Add Action"))
			{
				schedulerInstance.addAction(selectedCharacter, selectedAction);
			}

			for (int i = 0; i < MAX_CHARACTER_COUNT; i++)
			{
				if (schedulerInstance.m_actionQueues[i].size() > 0)
				{
					char buffer[100];
					sprintf_s(buffer, 100, "Character %d : %d actions", i, schedulerInstance.m_actionQueues[i].size());
					if (TreeNode(buffer))
					{
						typedActionData actions[MAX_ACTION_COUNT];
						schedulerInstance.m_actionQueues[i].getData(actions, MAX_ACTION_COUNT);
						for (int j = 0; j < schedulerInstance.m_actionQueues[i].size(); j++)
						{
							Text("(%s, duration : %d, direction : %s)", typeNames[actions[j].action].c_str(), actions[j].data.duration, ImGui::getDirectionName(actions[j].data.direction));
						}
						ImGui::TreePop();
					}
				}
			}

			if (Button("Add unused Character"))
				dispatcherInstance.m_unassignedCharacters.add(selectedCharacter);

			const character::handle* unassignedCharactersBuffer = dispatcherInstance.m_unassignedCharacters.getData();
			for (int i = 0; i < dispatcherInstance.m_unassignedCharacters.size(); i++)
			{
				Text("%d", unassignedCharactersBuffer[i]);
				SameLine();
			}

			if (Button("Update"))
			{
				dispatcherInstance.update(schedulerInstance);
				idleManager.handleInput();
				movingManager.handleInput();
				noneManager.handleInput();
				transform::move(movingManager, characterManager);
				idleManager.update();
				movingManager.update();
				noneManager.update();
			}



			Checkbox("Auto play", &isPlaying);
			SliderFloat("Auto Play fps", &autoPlayFps, 1, 60, "%.1f");

			if (isPlaying)
			{
				playTime += deltaTime;
				if (playTime > 1.0f / autoPlayFps)
				{
					playTime -= 1.0f / autoPlayFps;

					dispatcherInstance.update(schedulerInstance);
					idleManager.handleInput();
					movingManager.handleInput();
					noneManager.handleInput();
					transform::move(movingManager, characterManager);
					idleManager.update();
					movingManager.update();
					noneManager.update();
				}
			}

			drawAssignmentEditor("Idle", idleManager);
			drawAssignmentEditor("Moving", movingManager);

			world::drawWorldEditor(edData, characterManager, schedulerInstance, map);

			PopID();
			End();

			Begin("Characters Window");
			for (int i = 0; i < characterManager.count(); i++)
			{
				Text("Character %d : (%f, %f)", handles[i].id, positions[i].x, positions[i].y);
			}
			End();
		}
	}
}