#pragma once
#include "ComponentHeader.hpp"
#include "ECSConstants.hpp"
#include "ComponentHint.hpp"

namespace Core
{
	namespace DirectionalLight
	{
		struct Component
		{
			ComponentHeader header;
			Vec3 direction;
			Vec3 color;
			F32 intensity;
		};

		void OnImGui(ComponentHeader* pHeader);
		void OnSeralize(ComponentHeader* pHeader, std::ofstream& out, const String& entity);
		void Init(ComponentHeader* pHeader);

		int luaSetDirectionalLight(lua_State* L);

		inline static ComponentHint getHint()
		{
			return ComponentHint{ "DIRECTIONAL_LIGHT", sizeof(Component) };
		}
	}
}