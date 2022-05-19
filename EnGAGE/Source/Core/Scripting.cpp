#include "pch.hpp"
#include "Scripting.hpp"

#include "ECS.hpp"
#include "LuaHostFunctions.hpp"
#include "Messenger.hpp"
#include "Components/Script.hpp"

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

static bool checkLua(lua_State* L, int r);

//Lua scripts
static DynArr<lua_State*> gScripts;


void Core::Scripting::onMessage(const Message* pMessage)
{

	if (auto message = Messenger::messageCast<MessageType::REMOVE_SCRIPT, RemoveScriptMessage>(pMessage))
	{
		removeScript(message->L);
	}

	System& system = ECS::getSystem(SystemType::SCRIPTING);
	for (unsigned int entity : system.entities)
	{
		Script::Component* pScriptComponent = (Script::Component*)ECS::getComponent(entity, ComponentType::SCRIPT);
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

void Core::Scripting::update(float delta)
{
	System& system = ECS::getSystem(SystemType::SCRIPTING);
	for (unsigned int entity : system.entities)
	{
		Script::Component* pScriptComponent = (Script::Component*)ECS::getComponent(entity, ComponentType::SCRIPT);
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

void Core::Scripting::clear()
{
	for (const auto& L : gScripts)
	{
		lua_close(L);
	}

	gScripts.clear();
}

lua_State* Core::Scripting::newScript()
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);

	//Register all host functions
	LuaHostFunctions::pushAllGlobals(L);
	LuaHostFunctions::registerFunctions(L);
	gScripts.push_back(L);
	return L;
}

void Core::Scripting::loadFile(lua_State* L, const String path)
{
	EN_ASSERT(L != nullptr, "L ptr is null");

	if (std::find(gScripts.begin(), gScripts.end(), L) == gScripts.end())
	{
		EN_ASSERT(false, "Lua VM not added !");
		return;
	}
	checkLua(L, luaL_dofile(L, path.c_str()));
}

void Core::Scripting::removeScript(lua_State* L)
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

