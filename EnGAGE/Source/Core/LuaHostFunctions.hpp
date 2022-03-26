#pragma once

struct lua_State;

namespace LuaHostFunctions
{
	int keyPressed(lua_State* L);
	int keyPressedOnce(lua_State* L);
	int buttonPressed(lua_State* L);
	int getCursorPosDelta(lua_State* L);
	int toggleCursor(lua_State* L);
	int isCursorLocked(lua_State* L);

	int getComponent(lua_State* L);
	int setPosition(lua_State* L);
	int setRotation(lua_State* L);
	int setScale(lua_State* L);
	int translate(lua_State* L);
	int rotate(lua_State* L);
	int scale(lua_State* L);

	int updateCamera(lua_State* L);
}