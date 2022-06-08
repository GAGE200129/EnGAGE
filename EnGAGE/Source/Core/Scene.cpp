#include "pch.hpp"
#include "Scene.hpp"

#include "LuaHostFunctions.hpp"
#include "ECS.hpp"
#include "GameEngine.hpp"
#include "Physics.hpp"
#include "Math.hpp"
#include "Core/Data/Model.hpp"
#include "Core/Data/Vertex.hpp"
#include "Map/Map.hpp"
#include "Components/RigidBody.hpp"
#include "Components/Transform.hpp"
#include "Components/ModelRenderer.hpp"
#include "Components/Script.hpp"
#include "Components/DirectionalLight.hpp"

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


	void loadScene(const String& filePath)
	{
		gSceneSwitch = true;
		gScenePath = filePath;
	}

	void saveScene(const String& filePath)
	{
		std::ofstream fileOut(filePath);

		for (unsigned int i = 0; i < ECS::getEntityCount(); i++)
		{
			String entityName = "entity";
			fileOut << entityName << " = _createEntity()\n";

			//Unload all components
			for (unsigned int j = 0; j < (unsigned int)ComponentType::COUNT; j++)
			{
				auto& signatures = ECS::getEntitySignatures();
				ComponentHeader* pHeader = (ComponentHeader*)ECS::getComponent(signatures[i].id, (ComponentType)j);
				if (pHeader)
				{
					pHeader->OnSeralize(pHeader, fileOut, entityName);
				}
			}
			fileOut << "\n";
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
}
