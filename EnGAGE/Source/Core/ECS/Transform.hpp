#pragma once
#include "ComponentHeader.hpp"
#include "ECSConstants.hpp"
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

		int luaSetPosition(lua_State* L);
		int luaSetRotation(lua_State* L);
		int luaSetScale(lua_State* L);
		int luaGetTransform(lua_State* L);

		inline static ComponentHint getHint()
		{
			return ComponentHint{ "TRANSFORM", sizeof(Component) };
		}
	}
}