#pragma once
#include "ComponentHeader.hpp"
#include "Core/ECSConstants.hpp"
#include "ComponentHint.hpp"

namespace Core
{
	namespace Transform
	{
		struct Component
		{
			ComponentHeader header;
			F32 x, y, z,
				rw, rx, ry, rz,
				sx, sy, sz;
		};


		void OnImGui(ComponentHeader* pHeader);
		void OnSeralize(ComponentHeader* pComponent, std::ofstream& out, const String& entity);
		void Init(ComponentHeader* pHeader);

		inline static ComponentHint getHint()
		{
			return ComponentHint{ "TRANSFORM", sizeof(Component) };
		}
	}
}