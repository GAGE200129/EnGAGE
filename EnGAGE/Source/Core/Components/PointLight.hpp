#pragma once
#include "ComponentHeader.hpp"
#include "Core/ECSConstants.hpp"
#include "ComponentHint.hpp"
namespace Core
{
	namespace PointLight
	{
		struct Component
		{
			ComponentHeader header;
			Vec3 color;
			F32 intensity;

			F32 constant;
			F32 linear;
			F32 exponent;
		};


		void OnImGui(ComponentHeader* pHeader);
		void OnSeralize(ComponentHeader* pHeader, std::ofstream& out, const String& entity);
		void Init(ComponentHeader* pHeader);

		inline static ComponentHint getHint()
		{
			return ComponentHint{ "POINT_LIGHT", sizeof(Component) };
		}
	}
}