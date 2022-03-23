#pragma once

struct lua_State;
namespace Core
{
	namespace Lua
	{
		void update(float delta);
		void shutdown();
		lua_State* newScript(unsigned int entity);
		void loadFile(lua_State* L, const String path);
		void removeScript(lua_State* L);
	}
}