#pragma once

struct lua_State;

namespace LuaHostFunctions
{
	void registerAllFunctions(lua_State* L);
	
}