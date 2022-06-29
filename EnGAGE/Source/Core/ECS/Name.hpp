#pragma once

#include "ComponentHeader.hpp"
#include "ECSConstants.hpp"
#include "ComponentHint.hpp"
namespace Core
{
	namespace Name
	{
		struct Component
		{
			ComponentHeader header;
			Byte name[ECS::MAX_NAME_SIZE];
		};

		void OnImGui(ComponentHeader* pHeader);
		void OnSeralize(ComponentHeader* pHeader, std::ofstream& out, const String& entity);
		void Init(ComponentHeader* pHeader);
		inline static ComponentHint getHint()
		{
			return ComponentHint{ "NAME", sizeof(Component) };
		}
	}
}