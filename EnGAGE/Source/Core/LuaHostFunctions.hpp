#pragma once

struct lua_State;

namespace Core::LuaHostFunctions
{
	void registerFunctions(lua_State* L);
	void pushAllGlobals(lua_State* L);
}