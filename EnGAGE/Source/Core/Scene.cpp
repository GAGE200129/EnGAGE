#include "pch.hpp"
#include "Scene.hpp"

#include "LuaHostFunctions.hpp"
#include "ECS.hpp"
#include "GameEngine.hpp"

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}
static bool checkLua(lua_State* L, int r);

void Core::Scene::loadScene(const String& filePath)
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);

	for (unsigned int i = 0; i < (unsigned int)ECS::ComponentType::COUNT; i++)
	{
		ECS::ComponentData data = ECS::getComponentData((ECS::ComponentType)i);
		lua_pushinteger(L, i);
		lua_setglobal(L, data.name);
	}
	LuaHostFunctions::registerAllFunctions(L);
	GameEngine::clearResources();

	checkLua(L, luaL_dofile(L, filePath.c_str()));

	lua_close(L);
}


static bool checkLua(lua_State* L, int r)
{
	if (r != LUA_OK)
	{
		EN_ERROR("Lua error: {}", lua_tostring(L, -1));
		return false;
	}

	return true;
}
