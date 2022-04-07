#include "pch.hpp"
#include "Script.hpp"

#include "ECS.hpp"
#include "LuaHostFunctions.hpp"
#include "InputCodes.hpp"

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

static bool checkLua(lua_State* L, int r);
static void setIntegerGlobal(lua_State* L, const String& name, unsigned int num);

//Lua scripts
static DynArr<lua_State*> gScripts;

void Core::Script::input()
{
	ECS::System& system = ECS::getSystem(ECS::SystemType::SCRIPTING);
	for (unsigned int entity : system.entities)
	{
		ECS::ScriptComponent* pScriptComponent = (ECS::ScriptComponent*)ECS::getComponent(entity, ECS::ComponentType::SCRIPT);
		lua_State* L = pScriptComponent->L;
		lua_getglobal(L, "input");
		if (lua_isfunction(L, -1))
		{
			lua_pushinteger(L, entity);
			checkLua(L, lua_pcall(L, 1, 0, 0));
		}
		lua_settop(L, 0);
	}
}

void Core::Script::update(float delta)
{
	ECS::System& system = ECS::getSystem(ECS::SystemType::SCRIPTING);
	for (unsigned int entity : system.entities)
	{
		ECS::ScriptComponent* pScriptComponent = (ECS::ScriptComponent*)ECS::getComponent(entity, ECS::ComponentType::SCRIPT);
		lua_State* L = pScriptComponent->L;

		lua_getglobal(L, "update");
		if (lua_isfunction(L, -1))
		{
			lua_pushnumber(L, delta);
			lua_pushinteger(L, entity);
			checkLua(L, lua_pcall(L, 2, 0, 0));
		}
		lua_settop(L, 0);
	}
}

void Core::Script::shutdown()
{
	for (const auto& L : gScripts)
	{
		lua_close(L);
	}

	gScripts.clear();
}

lua_State* Core::Script::newScript(unsigned int entity)
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);

	//Register all host functions
	LuaHostFunctions::registerAllScriptFunctions(L);
	
	//Add globals
	for (unsigned int i = 0; i < (unsigned int)ECS::ComponentType::COUNT; i++)
	{
		ECS::ComponentData data = ECS::getComponentData((ECS::ComponentType)i);
		setIntegerGlobal(L, data.name, i);
	}

	for (unsigned int i = 0; i < InputCodes::NUM_KEYS; i++)
	{
		const char* name = InputCodes::toString(i);
		if (strlen(name) != 0)
		{
			setIntegerGlobal(L, name, i);
		}
	}


	gScripts.push_back(L);
	return L;
}

void Core::Script::loadFile(lua_State* L, const String path)
{
	EN_ASSERT(L != nullptr, "L ptr is null");

	if (std::find(gScripts.begin(), gScripts.end(), L) == gScripts.end())
	{
		EN_ASSERT(false, "Lua VM not added !");
		return;
	}
	checkLua(L, luaL_dofile(L, path.c_str()));
}

void Core::Script::removeScript(lua_State* L)
{
	gScripts.erase(std::remove(gScripts.begin(), gScripts.end(), L), gScripts.end());
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

