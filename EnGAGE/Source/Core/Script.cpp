#include "pch.hpp"
#include "Script.hpp"

#include "ECS.hpp"
#include "LuaHostFunctions.hpp"
#include "InputCodes.hpp"
#include "Physics.hpp"
#include "Messenger.hpp"

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

static bool checkLua(lua_State* L, int r);

//Lua scripts
static DynArr<lua_State*> gScripts;


void Core::Script::onMessage(const Message* pMessage)
{
	switch (pMessage->type)
	{
	case MessageType::REMOVE_SCRIPT:
	{
		removeScript((lua_State*)pMessage->message);
		break;
	}
	}

	System& system = ECS::getSystem(SystemType::SCRIPTING);
	for (unsigned int entity : system.entities)
	{
		ScriptComponent* pScriptComponent = (ScriptComponent*)ECS::getComponent(entity, ComponentType::SCRIPT);
		lua_State* L = pScriptComponent->L;
		lua_getglobal(L, "onMessage");
		if (lua_isfunction(L, -1))
		{
			lua_pushinteger(L, entity);
			lua_pushinteger(L, (lua_Integer)pMessage->type);
			lua_pushlstring(L, (char*)pMessage->message, Messenger::BUFFER_SIZE);

			checkLua(L, lua_pcall(L, 3, 0, 0));
		}
		lua_settop(L, 0);
	}
}

void Core::Script::onRequest(Request* pRequest)
{
	switch(pRequest->type)
	{
	case RequestType::NEW_SCRIPT:
	{
		lua_State* L = newScript();
		memcpy(pRequest->data, &L, sizeof(lua_State*));
		return;
	}
	}
}

void Core::Script::update(float delta)
{
	System& system = ECS::getSystem(SystemType::SCRIPTING);
	for (unsigned int entity : system.entities)
	{
		ScriptComponent* pScriptComponent = (ScriptComponent*)ECS::getComponent(entity, ComponentType::SCRIPT);
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

lua_State* Core::Script::newScript()
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);

	//Register all host functions
	LuaHostFunctions::registerAllScriptFunctions(L);
	
	//Add globals
	for (unsigned int i = 0; i < (unsigned int)ComponentType::COUNT; i++)
	{
		const ComponentData& data = getComponentData((ComponentType)i);
		lua_pushinteger(L, i);
		lua_setglobal(L, data.name);
	}


	for (unsigned int i = 0; i < (unsigned int)MessageType::COUNT; i++)
	{
		const char* name = getMessageName((MessageType)i);
		lua_pushinteger(L, i);
		lua_setglobal(L, name);
	}

	for (unsigned int i = 0; i < InputCodes::NUM_KEYS; i++)
	{
		const char* name = InputCodes::toString(i);
		if (strlen(name) != 0)
		{
			lua_pushinteger(L, i);
			lua_setglobal(L, name);
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


