#include "geometry.h"
#include "character.h"

namespace character
{
	void manager::move(const handle* characters, vec2* movements, int count)
	{
		for (int i = 0; i < count; i++)
		{
			m_data[m_indirs[characters[i].id]] += movements[i];
		}
	}
}