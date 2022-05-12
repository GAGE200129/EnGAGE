#pragma once

#include "MessageTypes.hpp"

struct lua_State;
namespace Core::Scripting
{
	void onMessage(const Message* pMessage);
	void update(float delta);
	void shutdown();
	lua_State* newScript();
	void loadFile(lua_State* L, const String path);
	void removeScript(lua_State* L);
}