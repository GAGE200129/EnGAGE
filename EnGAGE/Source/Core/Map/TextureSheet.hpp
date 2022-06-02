#pragma once

namespace Core
{
	struct TextureSheet
	{
		String name;
		UInt32 width, height;
		UInt32 id;
		Vec2I div;
	};
}