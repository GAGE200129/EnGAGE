#include "pch.hpp"
#include "Lua.hpp"

#include "ECS.hpp"
#include "LuaHostFunctions.hpp"

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

static bool checkLua(lua_State* L, int r);
static void setIntegerGlobal(lua_State* L, const String& name, unsigned int num);

//Lua scripts
static DynArr<lua_State*> gLuaVMs;



void Core::Lua::update(float delta)
{
	for (lua_State* L : gLuaVMs)
	{
		lua_getglobal(L, "update");
		if (lua_isfunction(L, -1))
		{
			lua_pushnumber(L, delta);
			checkLua(L, lua_pcall(L, 1, 0, 0));
		}
		lua_settop(L, 0);
	}
}

void Core::Lua::shutdown()
{
	for (const auto& L : gLuaVMs)
	{
		lua_close(L);
	}
}

lua_State* Core::Lua::newScript(unsigned int entity)
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	//Push entity id constants
	setIntegerGlobal(L, "entity", entity);

	//Register all host functions
	lua_register(L, "_getComponent", lua_getComponent);
	lua_register(L, "_updateComponent", lua_updateComponent);
	
	//Add globals
	for (unsigned int i = 0; i < (unsigned int)ECS::ComponentType::COUNT; i++)
	{
		ECS::ComponentData data = ECS::getComponentData((ECS::ComponentType)i);
		setIntegerGlobal(L, data.name, i);
	}


	gLuaVMs.push_back(L);
	return L;
}

void Core::Lua::loadFile(lua_State* L, const String path)
{
	EN_ASSERT(L != nullptr, "L ptr is null");

	if (std::find(gLuaVMs.begin(), gLuaVMs.end(), L) == gLuaVMs.end())
	{
		EN_ASSERT(false, "Lua VM not added !");
		return;
	}
	checkLua(L, luaL_dofile(L, path.c_str()));
}

void Core::Lua::removeScript(lua_State* L)
{
	gLuaVMs.erase(std::remove(gLuaVMs.begin(), gLuaVMs.end(), L), gLuaVMs.end());
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

static void setIntegerGlobal(lua_State* L, const String& name, unsigned int num)
{
	lua_pushinteger(L, num);
	lua_setglobal(L, name.c_str());
}

