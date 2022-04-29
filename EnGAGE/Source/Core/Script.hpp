#pragma once

#include "MessageTypes.hpp"
#include "RequestTypes.hpp"

struct lua_State;
namespace Core::Script
{
	void onMessage(const Message* pMessage);
	bool onRequest(Request* pRequest);
	void update(float delta);
	void shutdown();
	lua_State* newScript();
	void loadFile(lua_State* L, const String path);
	void removeScript(lua_State* L);
}