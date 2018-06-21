#pragma once
#include "geometry.h"
#include "util/packed_array.h"

#define MAX_CHARACTER_COUNT 100

namespace character
{
	using handle = parray<vec2, MAX_CHARACTER_COUNT>::handle;
	class manager : public parray<vec2, MAX_CHARACTER_COUNT>
	{
	public:
		void move(const handle* characters, vec2* movement, int count);
	};
}