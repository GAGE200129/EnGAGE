#pragma once

struct lua_State;

namespace LuaHostFunctions
{
	void registerAllScriptFunctions(lua_State* L);
	void registerAllSceneFunctions(lua_State* L);
}