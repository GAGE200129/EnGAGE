#include "pch.hpp"
#include "Core/GameEngine.hpp"
#include "Core/ECS.hpp"
#include "Core/Resource.hpp"

extern "C"
{
	#include <lua.h>
	#include <lauxlib.h>
	#include <lualib.h>
}

int main()
{
	std::string cmd = "a = 7 + 11";

	lua_State* l = luaL_newstate();
	int r = luaL_dostring(l, cmd.c_str());

	if (r == LUA_OK)
	{
		lua_getglobal(l, "a");
		if (lua_isnumber(l, -1))
		{
			float result = lua_tonumber(l, -1);
			std::cout << result << "\n";
		}
	}

	lua_close(l);

	using namespace Core;
	GameEngine::init(1600, 900, "Hello world");
	GameEngine::run();
}