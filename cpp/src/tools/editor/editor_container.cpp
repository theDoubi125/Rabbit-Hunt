#include "editor_container.h"
#include "imgui/imgui.h"
#include "editor_main.h"
#include "util/packed_array.h"
#include "util/sorted_array.h"
#include "memory/allocator.h"
#include "util/buffer.h"

#include <cstdio>
#include <vector>

using namespace ImGui;

namespace editor
{
	namespace container
	{
		char buf[100];
		using handle = parray<char, 100>::handle;
		struct HandleValue
		{
			handle arrayHandle;
			char value;
		};
		void drawPackedArrayEditor()
		{
			Begin("Packed Array", &pArraySelected, ImGuiWindowFlags_AlwaysAutoResize);
			static parray<char, 100> array;
			static std::vector<HandleValue> values;

			for (int i = 0; i < array.count(); i++)
			{
				Text("%c", array.m_data[i]);
			}

			static char value = 'a';
			sprintf_s(buf, "%c", value);
			if (BeginCombo("Value Combo", buf))
			{
				for (int i = 0; i < 26; i++)
				{
					sprintf_s(buf, "%c", 'a' + i);
					if (Selectable(buf, 'a' + i == value))
					{
						value = 'a' + i;
					}
				}
				EndCombo();
			}
			if (Button("Add"))
			{
				handle newHandle = array.add(value);
				values.push_back({ newHandle , value });
			}

			static handle selectedHandle;
			sprintf_s(buf, "%d", selectedHandle.id);
			if (BeginCombo("To Remove Combo", buf))
			{
				for (int i = 0; i < values.size(); i++)
				{
					sprintf_s(buf, "(%d, %c)", values[i].arrayHandle.id, values[i].value);
					if (Selectable(buf, selectedHandle == values[i].arrayHandle))
					{
						selectedHandle = values[i].arrayHandle;
					}
				}
				EndCombo();
			}
			if (Button("Remove"))
			{
				array.remove(selectedHandle);
				for (auto it = values.begin(); it != values.end(); it++)
				{
					if (it->arrayHandle == selectedHandle)
					{
						values.erase(it);
						break;
					}
				}
			}

			End();
		}

		void drawAllocArrayEditor()
		{
			static char t[200];
			static int size = 0;

			Begin("Packed Array", &pArraySelected, ImGuiWindowFlags_AlwaysAutoResize);
			for (int i = 0; i < size; i++)
			{
				Text("%c", t[i]);
			}

			static char value = 'a';
			sprintf_s(buf, "%c", value);
			if (BeginCombo("Value Combo", buf))
			{
				for (int i = 0; i < 26; i++)
				{
					sprintf_s(buf, "%c", 'a' + i);
					if (Selectable(buf, 'a' + i == value))
					{
						value = 'a' + i;
					}
				}
				EndCombo();
			}
			if (Button("Add"))
			{
				array::addSorted(t, value, size++);
			}

			End();
		}

		void drawMemoryAllocatorEditor()
		{
			memory::allocator allocator(1000000);
			buffer<int> test = allocator.allocateBuffer<int>(200);
			buffer<int> test2 = allocator.allocateBuffer<int>(200);
			size_t diff = (char*)test2.getData() - (char*)test.getData();
		}
	}
}