#include "pch.hpp"
#include "Lua.hpp"

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
static DynArr<lua_State*> gLuaVMs;

void Core::Lua::input()
{
	for (unsigned int i = 0; i < gLuaVMs.size(); i++)
	{
		lua_State* L = gLuaVMs[i];
		lua_getglobal(L, "input");
		if (lua_isfunction(L, -1))
		{
			checkLua(L, lua_pcall(L, 0, 0, 0));
		}
		lua_settop(L, 0);
	}
}

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
	setIntegerGlobal(L, "_entity", entity);

	//Register all host functions
	lua_register(L, "_createEntity", LuaHostFunctions::createEntity);
	lua_register(L, "_markForRemove", LuaHostFunctions::markForRemove);
	lua_register(L, "_getComponent", LuaHostFunctions::getComponent);
	lua_register(L, "_addComponent", LuaHostFunctions::addComponent);

	lua_register(L, "_keyPressed", LuaHostFunctions::keyPressed);
	lua_register(L, "_keyPressedOnce", LuaHostFunctions::keyPressedOnce);
	lua_register(L, "_buttonPressed", LuaHostFunctions::buttonPressed);
	lua_register(L, "_getCursorPosDelta", LuaHostFunctions::getCursorPosDelta);
	lua_register(L, "_toggleCursor", LuaHostFunctions::toggleCursor);
	lua_register(L, "_isCursorLocked", LuaHostFunctions::isCursorLocked);
	lua_register(L, "_setPosition", LuaHostFunctions::setPosition);
	lua_register(L, "_setRotation", LuaHostFunctions::setRotation);
	lua_register(L, "_setScale", LuaHostFunctions::setScale);
	lua_register(L, "_translate", LuaHostFunctions::translate);
	lua_register(L, "_rotate", LuaHostFunctions::rotate);
	lua_register(L, "_scale", LuaHostFunctions::scale);
	lua_register(L, "_updateCamera", LuaHostFunctions::updateCamera);
	lua_register(L, "_setModel", LuaHostFunctions::setModel);
	lua_register(L, "_setRigidBody", LuaHostFunctions::setRigidBody);
	lua_register(L, "_setCollisionShapeSphere", LuaHostFunctions::setCollisionShapeSphere);
	lua_register(L, "_getVelocity", LuaHostFunctions::getVelocity);
	lua_register(L, "_setScript", LuaHostFunctions::setScript);
	
	//Add globals
	for (unsigned int i = 0; i < (unsigned int)ECS::ComponentType::COUNT; i++)
	{
		ECS::ComponentData data = ECS::getComponentData((ECS::ComponentType)i);
		setIntegerGlobal(L, data.name, i);
	}

	for (unsigned int i = 0; i < InputCodes::NUM_KEYS; i++)
	{
		const char* name = InputCodes::toString(i);
		if (name != "")
		{
			setIntegerGlobal(L, name, i);
		}
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

