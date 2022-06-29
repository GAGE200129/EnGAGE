#pragma once

struct lua_State;

namespace Core::LuaHostFunctions
{
	void checkNumArguments(lua_State* L, unsigned int arguments, const char* functionName);
	void checkArgument(lua_State* L, int index, unsigned int expectedType, const char* functionName);
	void registerFunctions(lua_State* L);
	void pushAllGlobals(lua_State* L);
}

#ifdef EN_DEBUG
#define CHECK_NUM_ARGS(L, numArgs) Core::LuaHostFunctions::checkNumArguments(L, numArgs, __FUNCTION__)
#define CHECK_ARG(L, index, type) Core::LuaHostFunctions::checkArgument(L, index, type, __FUNCTION__)
#else
#define CHECK_NUM_ARGS(L, numArgs)
#define CHECK_ARG(L, index, type)
#endif