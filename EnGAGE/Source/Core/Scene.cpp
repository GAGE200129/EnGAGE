#include "pch.hpp"
#include "Scene.hpp"

#include "LuaHostFunctions.hpp"
#include "Core/ECS/ECS.hpp"
#include "GameEngine.hpp"
#include "Core/Physics/Physics.hpp"
#include "Math.hpp"
#include "Core/Data/Model.hpp"
#include "Core/Data/Vertex.hpp"
#include "Map/Map.hpp"
#include "ECS/RigidBody.hpp"
#include "ECS/Transform.hpp"
#include "ECS/ModelRenderer.hpp"
#include "ECS/Script.hpp"
#include "ECS/DirectionalLight.hpp"

#include <glad/glad.h>

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
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

namespace Core::Scene
{
	static bool gSceneSwitch = false;
	static String gScenePath = "";
	static DynArr<String> gPresets;

	void loadPreset(const String& filePath)
	{
		lua_State* L = luaL_newstate();

		LuaHostFunctions::pushAllGlobals(L);
		LuaHostFunctions::registerFunctions(L);

		checkLua(L, luaL_dofile(L, filePath.c_str()));

		lua_getglobal(L, "create");
		if (lua_isfunction(L, -1))
		{
			checkLua(L, lua_pcall(L, 0, 0, 0));
		}

		lua_close(L);


		gPresets.push_back(String(filePath));
	}

	void savePreset(const String& filePath, UInt32 entityID)
	{
		std::ofstream fileOut(filePath);

		fileOut << "function create()\n";

		String entityName = "entity";
		fileOut << "local " << entityName << " = _createEntity()\n";

		//Unload all components
		for (unsigned int j = 0; j < (unsigned int)ComponentType::COUNT; j++)
		{
			auto& signatures = ECS::getEntitySignatures();
			ComponentHeader* pHeader = (ComponentHeader*)ECS::getComponent(entityID, (ComponentType)j);
			if (pHeader)
			{
				pHeader->OnSeralize(pHeader, fileOut, entityName);
			}
		}
		fileOut << "end\n";

		fileOut.close();
	}

	void loadScene(const String& filePath)
	{
		gSceneSwitch = true;
		gScenePath = filePath;
	}

	void saveScene(const String& filePath)
	{
		std::ofstream fileOut(filePath);

		for (const auto& preset : gPresets)
		{
			fileOut << "_sceneDoPreset(\"" << preset << "\")\n";
		}



		//Serialize the map
		Map::serialize(fileOut);

		fileOut.close();
	}

	void checkForSceneSwitch()
	{
		if (gSceneSwitch)
		{
			EN_INFO("Switching scene: {}", gScenePath);
			GameEngine::clearResources();

			lua_State* L = luaL_newstate();
			luaL_openlibs(L);

			LuaHostFunctions::pushAllGlobals(L);
			LuaHostFunctions::registerFunctions(L);

			checkLua(L, luaL_dofile(L, gScenePath.c_str()));

			lua_close(L);

			gSceneSwitch = false;
		}
	}

	String& getLoadedSceneName()
	{
		return gScenePath;
	}
	int luaDoPreset(lua_State* L)
	{
		CHECK_NUM_ARGS(L, 1);
		CHECK_ARG(L, 1, LUA_TSTRING);
		const char* filePath = lua_tostring(L, 1);
		loadPreset(filePath);

		return 0;
	}
}
