#pragma once
#include "ComponentHeader.hpp"
#include "Core/ECSConstants.hpp"
#include "ComponentHint.hpp"

struct lua_State;
namespace Core
{
	namespace Script
	{
		struct Component
		{
			ComponentHeader header;
			Byte scriptPath[ECS::MAX_NAME_SIZE];
			lua_State* L;
		};


		void OnImGui(ComponentHeader* pHeader);
		void OnSeralize(ComponentHeader* pComponent, std::ofstream& out, const String& entity);
		void Destroy(ComponentHeader* pHeader);
		void Init(ComponentHeader* pHeader);
		inline static ComponentHint getHint()
		{
			return ComponentHint{ "SCRIPT", sizeof(Component) };
		}
	}
}