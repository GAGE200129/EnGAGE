#include "pch.hpp"
#include "Script.hpp"

#include "ECS.hpp"
#include "LuaHostFunctions.hpp"
#include "InputCodes.hpp"
#include "Physics.hpp"

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

static bool checkLua(lua_State* L, int r);

//Lua scripts
static DynArr<lua_State*> gScripts;


void Core::Script::onMessage(const Messaging::Message* pMessage)
{
	ECS::System& system = ECS::getSystem(ECS::SystemType::SCRIPTING);
	for (unsigned int entity : system.entities)
	{
		ECS::ScriptComponent* pScriptComponent = (ECS::ScriptComponent*)ECS::getComponent(entity, ECS::ComponentType::SCRIPT);
		lua_State* L = pScriptComponent->L;
		lua_getglobal(L, "onMessage");
		if (lua_isfunction(L, -1))
		{
			lua_pushinteger(L, entity);
			lua_pushinteger(L, (lua_Integer)pMessage->type);
			lua_pushlstring(L, pMessage->message, Messaging::BUFFER_SIZE);

			checkLua(L, lua_pcall(L, 3, 0, 0));
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

lua_State* Core::Script::newScript()
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);

	//Register all host functions
	LuaHostFunctions::registerAllScriptFunctions(L);
	
	//Add globals
	for (unsigned int i = 0; i < (unsigned int)ECS::ComponentType::COUNT; i++)
	{
		const ECS::ComponentData& data = ECS::getComponentData((ECS::ComponentType)i);
		lua_pushinteger(L, i);
		lua_setglobal(L, data.name);
	}

	for (unsigned int i = 0; i < (unsigned int)Physics::ColliderType::COUNT; i++)
	{
		const Physics::ColliderData& data = Physics::getColliderData((Physics::ColliderType)i);
		lua_pushinteger(L, i);
		lua_setglobal(L, data.name);
	}

	for (unsigned int i = 0; i < (unsigned int)Messaging::MessageType::COUNT; i++)
	{
		const char* name = Messaging::getMessageName((Messaging::MessageType)i);
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


