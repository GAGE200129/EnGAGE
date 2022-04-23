#pragma once

#include "Messaging.hpp"

struct lua_State;
namespace Core::Script
{
	void onMessage(const Messaging::Message* pMessage);
	void update(float delta);
	void shutdown();
	lua_State* newScript();
	void loadFile(lua_State* L, const String path);
	void removeScript(lua_State* L);
}